#include <algorithm>
#include <vector>
#include "plugin.hpp"
#include "sapphire_widget.hpp"
#include "tricorder.hpp"

// Tricorder for VCV Rack 2, by Don Cross <cosinekitty@gmail.com>
// https://github.com/cosinekitty/sapphire

namespace Sapphire
{
    namespace Tricorder
    {
        struct TricorderWidget;

        const int TRAIL_LENGTH = 1000;      // how many (x, y, z) points are held for the 3D plot

        struct Point
        {
            float x;
            float y;
            float z;

            Point()
                : x(0)
                , y(0)
                , z(0)
                {}

            Point(float _x, float _y, float _z)
                : x(_x)
                , y(_y)
                , z(_z)
                {}
        };

        using PointList = std::vector<Point>;

        class RotationMatrix
        {
        private:
            float rot[3][3];

        public:
            RotationMatrix()
            {
                initialize();
            }

            void initialize()
            {
                rot[0][0] = 1;
                rot[0][1] = 0;
                rot[0][2] = 0;
                rot[1][0] = 0;
                rot[1][1] = 1;
                rot[1][2] = 0;
                rot[2][0] = 0;
                rot[2][1] = 0;
                rot[2][2] = 1;
            }

            void pivot(
                unsigned axis,     // selects which axis to rotate around: 0=x, 1=y, 2=z
                float radians)     // rotation counterclockwise looking from the positive direction of `axis` toward the origin
            {
                float c = std::cos(radians);
                float s = std::sin(radians);

                // We need to maintain the "right-hand" rule, no matter which
                // axis was selected. That means we pick (i, j, k) axis order
                // such that the following vector cross product is satisfied:
                // i x j = k
                unsigned i = (axis + 1) % 3;
                unsigned j = (axis + 2) % 3;
                unsigned k = axis % 3;

                float t   = c*rot[i][i] - s*rot[i][j];
                rot[i][j] = s*rot[i][i] + c*rot[i][j];
                rot[i][i] = t;

                t         = c*rot[j][i] - s*rot[j][j];
                rot[j][j] = s*rot[j][i] + c*rot[j][j];
                rot[j][i] = t;

                t         = c*rot[k][i] - s*rot[k][j];
                rot[k][j] = s*rot[k][i] + c*rot[k][j];
                rot[k][i] = t;
            }

            Point rotate(const Point& p) const
            {
                float x = rot[0][0]*p.x + rot[1][0]*p.y + rot[2][0]*p.z;
                float y = rot[0][1]*p.x + rot[1][1]*p.y + rot[2][1]*p.z;
                float z = rot[0][2]*p.x + rot[1][2]*p.y + rot[2][2]*p.z;
                return Point(x, y, z);
            }
        };

        enum ParamId
        {
            PARAMS_LEN
        };

        enum InputId
        {
            INPUTS_LEN
        };

        enum OutputId
        {
            OUTPUTS_LEN
        };

        enum LightId
        {
            LIGHTS_LEN
        };

        struct TricorderModule : Module
        {
            PointList pointList;
            int pointCount{};
            int nextPointIndex{};
            float xprev{};
            float yprev{};
            float zprev{};
            bool bypassing = false;

            TricorderModule()
            {
                pointList.resize(TRAIL_LENGTH);     // maintain fixed length for entire lifetime
                config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
                initialize();
            }

            void resetPointList()
            {
                pointCount = 0;
                nextPointIndex = 0;
                xprev = yprev = zprev = 0.0f;
            }

            void initialize()
            {
                resetPointList();
            }

            void onReset(const ResetEvent& e) override
            {
                Module::onReset(e);
                initialize();
            }

            void onBypass(const BypassEvent&) override
            {
                bypassing = true;
                resetPointList();
            }

            void onUnBypass(const UnBypassEvent&) override
            {
                bypassing = false;
            }

            static bool isCompatibleModule(const Module *module)
            {
                if (module == nullptr)
                    return false;

                if (module->model == modelFrolic)
                    return true;

                return false;
            }

            const Message* inboundMessage() const
            {
                if (isCompatibleModule(leftExpander.module))
                {
                    const Message* message = static_cast<const Message *>(leftExpander.module->rightExpander.consumerMessage);
                    if (IsValidMessage(message))
                        return message;
                }

                return nullptr;
            }

            static float filter(float v)
            {
                if (!std::isfinite(v))
                    return 0.0f;

                return std::max(-10.0f, std::min(+10.0f, v));
            }

            void process(const ProcessArgs& args) override
            {
                // Is a compatible module connected to the left?
                // If so, receive a triplet of voltages from it and put them in the buffer.
                const Message *msg = inboundMessage();
                if (msg == nullptr)
                {
                    // There is no compatible module flush to the left of Tricorder.
                    // Clear the display if it has stuff on it.
                    resetPointList();
                }
                else
                {
                    // Sanity check the values fed to us from the other module.
                    float x = filter(msg->x);
                    float y = filter(msg->y);
                    float z = filter(msg->z);
                    Point p(x, y, z);

                    // Only insert new points if the position has changed significantly
                    // or a sufficient amount of time has passed.
                    float dx = x - xprev;
                    float dy = y - yprev;
                    float dz = z - zprev;
                    float distance = std::sqrt(dx*dx + dy*dy + dz*dz);

                    if (distance > 0.1f)
                    {
                        if (pointCount < TRAIL_LENGTH)
                        {
                            pointList[pointCount++] = p;
                        }
                        else
                        {
                            pointList[nextPointIndex] = p;
                            nextPointIndex = (nextPointIndex + 1) % TRAIL_LENGTH;
                        }

                        // Because we added a point, reset the creteria for adding another point.
                        xprev = x;
                        yprev = y;
                        zprev = z;
                    }
                    else if (pointCount > 0)
                    {
                        // Instead of adding a new point, update the position of the most recently
                        // added point. This makes the animation much smoother.
                        if (pointCount < TRAIL_LENGTH)
                        {
                            pointList[pointCount-1] = p;
                        }
                        else
                        {
                            int latestPointIndex = (nextPointIndex + (TRAIL_LENGTH - 1)) % TRAIL_LENGTH;
                            pointList[latestPointIndex] = p;
                        }
                    }
                }
            }
        };


        enum class SegmentKind
        {
            Curve,
            Axis,
            Tip,
        };


        struct LineSegment
        {
            Vec vec1;           // rotated and scaled screen coordinates for first endpoint
            Vec vec2;           // rotated and scaled screen coordinates for second endpoint
            float prox;         // z-order proximity of segment's midpoint: larger values are closer to the viewer
            SegmentKind kind;   // what coloring rules to apply to the segment
            int index;          // index along the trail, used for fading the end; [1 .. TRAIL_LENGTH-1]

            LineSegment(Vec _vec1, Vec _vec2, float _prox, SegmentKind _kind, int _index)
                : vec1(_vec1)
                , vec2(_vec2)
                , prox(_prox)
                , kind(_kind)
                , index(_index)
                {}

            static LineSegment MakeTip(Vec vec, float prox)   // total hack!
            {
                return LineSegment(vec, vec, prox, SegmentKind::Tip, -1);
            }
        };

        inline bool operator < (const LineSegment& a, const LineSegment& b)      // needed for sorting renderList
        {
            // We want to sort the renderList in ascending order of the `prox` field.
            // The larger `prox` is, the closer the midpoint of the linesegment is to the viewer.
            // We want to draw farther line segments first, closer line segments last, so that
            // the closer line segments overlap the farther ones.
            return a.prox < b.prox;
        }

        using RenderList = std::vector<LineSegment>;


        struct TricorderDisplay : Widget
        {
            float radiansPerStep = -0.003f;
            float rotationRadians = 0.0f;
            float voltageScale = 5.0f;
            const float MM_SIZE = 105.0f;
            TricorderModule* module;
            TricorderWidget* parent;
            RotationMatrix orientation;
            RenderList renderList;

            TricorderDisplay(TricorderModule* _module, TricorderWidget* _parent)
                : module(_module)
                , parent(_parent)
            {
                box.pos = mm2px(Vec(10.5f, 12.0f));
                box.size = mm2px(Vec(MM_SIZE, MM_SIZE));
                orientation.pivot(0, 20.0*(M_PI/180.0));
            }

            void draw(const DrawArgs& args) override
            {
                math::Rect r = box.zeroPos();
                nvgBeginPath(args.vg);
                nvgRect(args.vg, RECT_ARGS(r));
                nvgFillColor(args.vg, SCHEME_BLACK);
                nvgFill(args.vg);
                Widget::draw(args);
            }

            void drawLayer(const DrawArgs& args, int layer) override
            {
                if (layer != 1)
                    return;

                if (module == nullptr || module->bypassing)
                    return;

                const int n = module->pointCount;
                if (n == 0)
                    return;

                renderList.clear();

                drawBackground();

                if (n < TRAIL_LENGTH)
                {
                    // The pointList has not yet reached full capacity.
                    // Render from the front to the back.
                    for (int i = 1; i < n; ++i)
                    {
                        const Point& p1 = module->pointList[i-1];
                        const Point& p2 = module->pointList[i];
                        addSegment(SegmentKind::Curve, i, p1, p2);
                    }
                    addTip(module->pointList[n-1]);
                }
                else
                {
                    // The pointList is full, so we treat it as a circular buffer.
                    int curr = module->nextPointIndex;      // the oldest point in the list
                    for (int i = 1; i < TRAIL_LENGTH; ++i)
                    {
                        int next = (curr + 1) % TRAIL_LENGTH;
                        const Point& p1 = module->pointList[curr];
                        const Point& p2 = module->pointList[next];
                        addSegment(SegmentKind::Curve, i, p1, p2);
                        curr = next;
                    }
                    addTip(module->pointList[curr]);
                }

                nvgSave(args.vg);
                Rect b = box.zeroPos();
                nvgScissor(args.vg, RECT_ARGS(b));
                render(args.vg, n);
                nvgResetScissor(args.vg);
                nvgRestore(args.vg);
            }

            void render(NVGcontext *vg, int pointCount)
            {
                // Sort in ascending order of line segment midpoint.
                std::sort(renderList.begin(), renderList.end());

                nvgLineCap(vg, NVG_ROUND);

                // Render in z-order to create correct blocking of segment visibility.
                for (const LineSegment& seg : renderList)
                {
                    if (seg.kind == SegmentKind::Tip)
                    {
                        nvgBeginPath(vg);
                        nvgStrokeColor(vg, SCHEME_WHITE);
                        nvgFillColor(vg, SCHEME_WHITE);
                        nvgCircle(vg, seg.vec1.x, seg.vec1.y, 1.5);
                        nvgFill(vg);
                    }
                    else
                    {
                        NVGcolor color = segmentColor(seg, pointCount);
                        float width = (seg.kind == SegmentKind::Axis) ? 1.0 : seg.prox/2 + 1.0f;
                        nvgBeginPath(vg);
                        nvgStrokeColor(vg, color);
                        nvgStrokeWidth(vg, width);
                        nvgMoveTo(vg, seg.vec1.x, seg.vec1.y);
                        nvgLineTo(vg, seg.vec2.x, seg.vec2.y);
                        nvgStroke(vg);
                    }
                }
            }

            NVGcolor segmentColor(const LineSegment& seg, int pointCount) const
            {
                NVGcolor nearColor;
                NVGcolor farColor = SCHEME_DARK_GRAY;
                switch (seg.kind)
                {
                case SegmentKind::Curve:
                    nearColor = SCHEME_CYAN;
                    farColor = SCHEME_PURPLE;
                    break;

                case SegmentKind::Axis:
                    nearColor = nvgRGB(0x90, 0x90, 0x60);
                    farColor = SCHEME_DARK_GRAY;
                    break;

                default:
                    nearColor = SCHEME_RED;
                    farColor = SCHEME_RED;
                    break;
                }

                float denom = static_cast<float>(std::max(40, pointCount));
                float factor = static_cast<float>(std::min(20, std::max(5, pointCount)));
                float opacity = (seg.index < 0) ? 1.0f : std::min(1.0f, (factor * seg.index) / denom);

                float prox = std::max(0.0f, std::min(1.0f, seg.prox));
                float dist = 1 - prox;
                NVGcolor color;
                color.a = 1;
                color.r = opacity*(prox*nearColor.r + dist*farColor.r);
                color.g = opacity*(prox*nearColor.g + dist*farColor.g);
                color.b = opacity*(prox*nearColor.b + dist*farColor.b);
                return color;
            }

            void addTip(const Point& point)
            {
                float prox;
                Vec vec = project(point, prox);
                // Give the tip a slight proximity bonus to account for its small radius.
                // We want to draw the tip after the line segment it is connected to.
                prox += 0.1;
                renderList.push_back(LineSegment::MakeTip(vec, prox));
            }

            void addSegment(SegmentKind kind, int index, const Point& point1, const Point& point2)
            {
                float prox1;
                Vec vec1 = project(point1, prox1);
                float prox2;
                Vec vec2 = project(point2, prox2);
                expandSegment(0, kind, index, vec1, vec2, prox1, prox2, point1, point2);
            }

            void expandSegment(
                int depth,
                SegmentKind kind,
                int index,
                const Vec& vec1,
                const Vec& vec2,
                float prox1,
                float prox2,
                const Point& point1,
                const Point& point2)
            {
                // If the endpoints are close enough to the same z-level (observer proximity)
                // or we have hit recursion depth limit, add a single line segment.
                if (depth == 5 || std::abs(prox1 - prox2) < 0.05f)
                {
                    renderList.push_back(LineSegment(vec1, vec2, (prox1+prox2)/2, kind, index));
                }
                else
                {
                    // Recursively split the line segment in two to handle inclination toward observer.
                    Point pointm((point1.x + point2.x)/2, (point1.y + point2.y)/2, (point1.z + point2.z)/2);
                    float proxm;
                    Vec vecm = project(pointm, proxm);
                    expandSegment(1+depth, kind, index, vec1, vecm, prox1, proxm, point1, pointm);
                    expandSegment(1+depth, kind, index, vecm, vec2, proxm, prox2, pointm, point2);
                }
            }

            void drawBackground()
            {
                const float r = 4.0f;
                Point origin(0, 0, 0);
                addSegment(SegmentKind::Axis, -1, origin, Point(r, 0, 0));
                addSegment(SegmentKind::Axis, -1, origin, Point(0, r, 0));
                addSegment(SegmentKind::Axis, -1, origin, Point(0, 0, r));
                drawLetterX(r);
                drawLetterY(r);
                drawLetterZ(r);
            }

            void drawLetterX(float r)
            {
                const float La = r * 1.04f;
                const float Lb = r * 1.14f;
                const float Lc = r * 0.02f;
                addSegment(SegmentKind::Axis, -1, Point(La, 0, -Lc), Point(Lb, 0, +Lc));
                addSegment(SegmentKind::Axis, -1, Point(La, 0, +Lc), Point(Lb, 0, -Lc));
            }

            void drawLetterY(float r)
            {
                const float La = r * 1.04f;
                const float Lb = r * 0.04f;
                const float Lc = r * 1.09f;
                const float Ld = r * 1.14f;
                addSegment(SegmentKind::Axis, -1, Point(0, Lc, 0), Point(  0, La, 0));
                addSegment(SegmentKind::Axis, -1, Point(0, Lc, 0), Point(-Lb, Ld, 0));
                addSegment(SegmentKind::Axis, -1, Point(0, Lc, 0), Point(+Lb, Ld, 0));
            }

            void drawLetterZ(float r)
            {
                const float La = r * 1.04f;
                const float Lb = r * 1.14f;
                const float Lc = r * 0.02f;
                addSegment(SegmentKind::Axis, -1, Point(-Lc, 0, La), Point(+Lc, 0, La));
                addSegment(SegmentKind::Axis, -1, Point(+Lc, 0, La), Point(-Lc, 0, Lb));
                addSegment(SegmentKind::Axis, -1, Point(-Lc, 0, Lb), Point(+Lc, 0, Lb));
            }

            Vec project(const Point& p, float& prox) const
            {
                // Apply the rotation matrix to the 3D point.
                Point q = orientation.rotate(p);

                // Project the 3D point 'p' onto a screen location Vec.
                float sx = (MM_SIZE/2) * (1 + q.x/voltageScale);
                float sy = (MM_SIZE/2) * (1 - q.y/voltageScale);
                prox = (1 + q.z/voltageScale) / 2;
                return mm2px(Vec(sx, sy));
            }

            void step() override
            {
                if (module == nullptr || module->bypassing)
                    return;

                rotationRadians = std::fmod(rotationRadians + radiansPerStep, 2*M_PI);
                orientation.initialize();
                orientation.pivot(1, rotationRadians);
                orientation.pivot(0, 23.5*(M_PI/180));
            }
        };


        struct TricorderWidget : SapphireReloadableModuleWidget
        {
            explicit TricorderWidget(TricorderModule *module)
                : SapphireReloadableModuleWidget(asset::plugin(pluginInstance, "res/tricorder.svg"))
            {
                setModule(module);

                // Load the SVG and place all controls at their correct coordinates.
                reloadPanel();

                addChild(new TricorderDisplay(module, this));
            }
        };
    }
}

Model *modelTricorder = createModel<Sapphire::Tricorder::TricorderModule, Sapphire::Tricorder::TricorderWidget>("Tricorder");
