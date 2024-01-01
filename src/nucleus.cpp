#include "plugin.hpp"
#include "sapphire_widget.hpp"
#include "nucleus_engine.hpp"

// Nucleus for VCV Rack 2, by Don Cross <cosinekitty@gmail.com>
// https://github.com/cosinekitty/sapphire

namespace Sapphire
{
    namespace Nucleus
    {
        const std::size_t NUM_PARTICLES = 5;

        enum ParamId
        {
            SPEED_KNOB_PARAM,
            DECAY_KNOB_PARAM,
            MAGNET_KNOB_PARAM,
            IN_DRIVE_KNOB_PARAM,
            OUT_LEVEL_KNOB_PARAM,

            SPEED_ATTEN_PARAM,
            DECAY_ATTEN_PARAM,
            MAGNET_ATTEN_PARAM,
            IN_DRIVE_ATTEN_PARAM,
            OUT_LEVEL_ATTEN_PARAM,

            PARAMS_LEN
        };

        enum InputId
        {
            X_INPUT,
            Y_INPUT,
            Z_INPUT,

            SPEED_CV_INPUT,
            DECAY_CV_INPUT,
            MAGNET_CV_INPUT,
            IN_DRIVE_CV_INPUT,
            OUT_LEVEL_CV_INPUT,

            INPUTS_LEN
        };

        enum OutputId
        {
            X1_OUTPUT,
            Y1_OUTPUT,
            Z1_OUTPUT,

            X2_OUTPUT,
            Y2_OUTPUT,
            Z2_OUTPUT,

            X3_OUTPUT,
            Y3_OUTPUT,
            Z3_OUTPUT,

            X4_OUTPUT,
            Y4_OUTPUT,
            Z4_OUTPUT,

            OUTPUTS_LEN
        };

        enum LightId
        {
            LIGHTS_LEN
        };

        struct NucleusModule : Module
        {
            NucleusEngine engine{NUM_PARTICLES};

            NucleusModule()
            {
                config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

                configInput(X_INPUT, "X");
                configInput(Y_INPUT, "Y");
                configInput(Z_INPUT, "Z");

                configParam(SPEED_KNOB_PARAM, -7, +7, 0, "Speed");
                configParam(DECAY_KNOB_PARAM, 0, 1, 0.5, "Decay");
                configParam(MAGNET_KNOB_PARAM, 0, 1, 0.5, "Magnetic coupling");
                configParam(IN_DRIVE_KNOB_PARAM, 0, 2, 1, "Input drive", " dB", -10, 80);
                configParam(OUT_LEVEL_KNOB_PARAM, 0, 2, 1, "Output level", " dB", -10, 80);

                configParam(SPEED_ATTEN_PARAM, -1, 1, 0, "Speed attenuverter", "%", 0, 100);
                configParam(DECAY_ATTEN_PARAM, -1, 1, 0, "Decay attenuverter", "%", 0, 100);
                configParam(MAGNET_ATTEN_PARAM, -1, 1, 0, "Magnetic coupling attenuverter", "%", 0, 100);
                configParam(IN_DRIVE_ATTEN_PARAM, -1, 1, 0, "Input drive attenuverter", "%", 0, 100);
                configParam(OUT_LEVEL_ATTEN_PARAM, -1, 1, 0, "Output level attenuverter", "%", 0, 100);

                configInput(SPEED_CV_INPUT, "Speed CV");
                configInput(DECAY_CV_INPUT, "Decay CV");
                configInput(MAGNET_CV_INPUT, "Magnetic coupling CV");
                configInput(IN_DRIVE_CV_INPUT, "Input level CV");
                configInput(OUT_LEVEL_CV_INPUT, "Output level CV");

                configOutput(X1_OUTPUT, "X1");
                configOutput(Y1_OUTPUT, "Y1");
                configOutput(Z1_OUTPUT, "Z1");
                configOutput(X2_OUTPUT, "X2");
                configOutput(Y2_OUTPUT, "Y2");
                configOutput(Z2_OUTPUT, "Z2");
                configOutput(X3_OUTPUT, "X3");
                configOutput(Y3_OUTPUT, "Y3");
                configOutput(Z3_OUTPUT, "Z3");
                configOutput(X4_OUTPUT, "X4");
                configOutput(Y4_OUTPUT, "Y4");
                configOutput(Z4_OUTPUT, "Z4");

                initialize();
            }

            void initParticles()
            {
                // The input ball [0] goes at the origin.
                engine.particle(0).pos = PhysicsVector::zero();

                // The remaining particles go at evenly spaced angles around the origin.
                // Alternate putting them slightly above/below the x-y plane.
                const float radius = 1.0;
                const float angleStep = (2.0 * M_PI) / NUM_PARTICLES;
                float angle = 0.0f;
                for (std::size_t i = 1; i < NUM_PARTICLES; ++i)
                {
                    Particle&p = engine.particle(i);
                    p.pos[0] = radius * std::cos(angle);
                    p.pos[1] = radius * std::sin(angle);
                    p.pos[2] = (((i%10)/9.0) - 0.5) * 1.0e-6f;
                    p.pos[3] = 0.0f;
                    p.vel = PhysicsVector::zero();
                    angle += angleStep;
                }
            }

            void initialize()
            {
                initParticles();
            }

            void onReset(const ResetEvent& e) override
            {
                Module::onReset(e);
                initialize();
            }

            float getControlValue(
                ParamId sliderId,
                ParamId attenuId,
                InputId cvInputId,
                float minSlider = 0.0f,
                float maxSlider = 1.0f)
            {
                float slider = params[sliderId].getValue();
                if (inputs[cvInputId].isConnected())
                {
                    float attenu = params[attenuId].getValue();
                    float cv = inputs[cvInputId].getVoltageSum();
                    // When the attenuverter is set to 100%, and the cv is +5V, we want
                    // to swing a slider that is all the way down (minSlider)
                    // to act like it is all the way up (maxSlider).
                    // Thus we allow the complete range of control for any CV whose
                    // range is [-5, +5] volts.
                    slider += attenu * (cv / 5.0) * (maxSlider - minSlider);
                }
                return slider;
            }

            void process(const ProcessArgs& args) override
            {
                engine.update();
            }
        };


        struct NucleusWidget : SapphireReloadableModuleWidget
        {
            explicit NucleusWidget(NucleusModule* module)
                : SapphireReloadableModuleWidget(asset::plugin(pluginInstance, "res/nucleus.svg"))
            {
                setModule(module);

                addSapphireInput(X_INPUT, "x_input");
                addSapphireInput(Y_INPUT, "y_input");
                addSapphireInput(Z_INPUT, "z_input");

                addSapphireOutput(X1_OUTPUT, "x1_output");
                addSapphireOutput(Y1_OUTPUT, "y1_output");
                addSapphireOutput(Z1_OUTPUT, "z1_output");
                addSapphireOutput(X2_OUTPUT, "x2_output");
                addSapphireOutput(Y2_OUTPUT, "y2_output");
                addSapphireOutput(Z2_OUTPUT, "z2_output");
                addSapphireOutput(X3_OUTPUT, "x3_output");
                addSapphireOutput(Y3_OUTPUT, "y3_output");
                addSapphireOutput(Z3_OUTPUT, "z3_output");
                addSapphireOutput(X4_OUTPUT, "x4_output");
                addSapphireOutput(Y4_OUTPUT, "y4_output");
                addSapphireOutput(Z4_OUTPUT, "z4_output");

                addKnob(SPEED_KNOB_PARAM, "speed_knob");
                addKnob(DECAY_KNOB_PARAM, "decay_knob");
                addKnob(MAGNET_KNOB_PARAM, "magnet_knob");
                addKnob(IN_DRIVE_KNOB_PARAM, "in_drive_knob");
                addKnob(OUT_LEVEL_KNOB_PARAM, "out_level_knob");

                addSapphireInput(SPEED_CV_INPUT, "speed_cv");
                addSapphireInput(DECAY_CV_INPUT, "decay_cv");
                addSapphireInput(MAGNET_CV_INPUT, "magnet_cv");
                addSapphireInput(IN_DRIVE_CV_INPUT, "in_drive_cv");
                addSapphireInput(OUT_LEVEL_CV_INPUT, "out_level_cv");

                addAttenuverter(SPEED_ATTEN_PARAM, "speed_atten");
                addAttenuverter(DECAY_ATTEN_PARAM, "decay_atten");
                addAttenuverter(MAGNET_ATTEN_PARAM, "magnet_atten");
                addAttenuverter(IN_DRIVE_ATTEN_PARAM, "in_drive_atten");
                addAttenuverter(OUT_LEVEL_ATTEN_PARAM, "out_level_atten");

                reloadPanel();
            }
        };
    }
}


Model* modelNucleus = createModel<Sapphire::Nucleus::NucleusModule, Sapphire::Nucleus::NucleusWidget>("Nucleus");