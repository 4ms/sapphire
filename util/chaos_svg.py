#!/usr/bin/env python3
#
#   chaos_svg.py  -  Don Cross <cosinekitty@gmail.com>
#
#   Generates the SVG panel design for Sapphire Frolic.
#
import sys
from svgpanel import *
from sapphire import *


def Print(message:str) -> int:
    print('chaos_svg.py:', message)
    return 0


def Save(panel:Panel, filename:str) -> int:
    panel.save(filename)
    return Print('Wrote: ' + filename)


def Gradient(y1: float, y2: float, color1: str, color2: str, id: str) -> Element:
    elem = Element('linearGradient', id)
    elem.setAttrib('x1', '0')
    elem.setAttribFloat('y1', y1)
    elem.setAttrib('x2', '0')
    elem.setAttribFloat('y2', y2)
    elem.setAttrib('gradientUnits', 'userSpaceOnUse')
    elem.append(Element('stop').setAttrib('style', 'stop-color:{};stop-opacity:1;'.format(color1)).setAttrib('offset', '0'))
    elem.append(Element('stop').setAttrib('style', 'stop-color:{};stop-opacity:1;'.format(color2)).setAttrib('offset', '1'))
    return elem


def ControlGroupArt(moduleName: str, id: str, panel: Panel, y1: float, y2: float, gradientId: str) -> Path:
    path = ''
    xMargin = 0.38
    arcRadius = 4.0
    x1 = xMargin
    x2 = panel.mmWidth - xMargin
    path += Move(x1, y2)
    path += Line(x1, y1 + arcRadius)
    # https://www.w3.org/TR/SVG2/paths.html#PathDataEllipticalArcCommands
    # arc for upper-left corner
    path += 'A {0:g} {0:g} 0 0 1 {1:g} {2:g} '.format(arcRadius, x1 + arcRadius, y1)
    path += Line(x2 - arcRadius, y1)
    # arc for upper-right corner
    if moduleName == 'glee':
        # right arc points upward, left arc points downward.
        path += 'A {0:g} {0:g} 0 0 0 {1:g} {2:g} '.format(arcRadius, x2, y1 - arcRadius)
    else:
        # both arcs point downward.
        path += 'A {0:g} {0:g} 0 0 1 {1:g} {2:g} '.format(arcRadius, x2, y1 + arcRadius)
    path += Line(x2, y1)
    path += Line(x2, y2)
    path += 'z'
    style = 'fill:url(#{});fill-opacity:1;stroke-width:0;'.format(gradientId)
    return Path(path, style, id)


def GenerateChaosPanel(name: str) -> int:
    PANEL_WIDTH = 4
    svgFileName = '../res/{}.svg'.format(name)
    panel = Panel(PANEL_WIDTH)
    pl = Element('g', 'PanelLayer')
    defs = Element('defs')
    pl.append(defs)
    panel.append(pl)
    controls = ControlLayer()
    pl.append(controls)
    with Font(SAPPHIRE_FONT_FILENAME) as font:
        pl.append(BorderRect(PANEL_WIDTH, SAPPHIRE_PANEL_COLOR, SAPPHIRE_BORDER_COLOR))
        pl.append(CenteredGemstone(panel))
        pl.append(ModelNamePath(panel, font, name))
        outputPortY1 = 91.5
        outputPortDY = 10.0
        xmid = panel.mmWidth / 2
        controls.append(Component('x_output', xmid, outputPortY1 + 0*outputPortDY))
        controls.append(Component('y_output', xmid, outputPortY1 + 1*outputPortDY))
        controls.append(Component('z_output', xmid, outputPortY1 + 2*outputPortDY))
        ySpeedKnob = 28.0
        yChaosKnob = 60.0
        dxControlGroup = 5.0
        dyControlGroup = 11.0
        dyControlText = -11.6
        xOutLabel = xmid - 3.9
        yOutLabel = outputPortY1 - 10.5
        artSpaceAboveKnob = 13.0
        artSpaceBelowKnob = 25.0
        xPortLabel = xmid - 7.5
        yPortLabel = outputPortY1 - 2.4
        outGradY1 = yOutLabel - 1.5
        outGradY2 = yPortLabel + 2*outputPortDY + 10.0
        defs.append(Gradient(ySpeedKnob-artSpaceAboveKnob, ySpeedKnob+artSpaceBelowKnob, '#3068ff', '#4f8df2', 'gradient_blue'))
        defs.append(Gradient(yChaosKnob-artSpaceAboveKnob, yChaosKnob+artSpaceBelowKnob, '#a06de4', '#4f8df2', 'gradient_purple'))
        defs.append(Gradient(outGradY1, outGradY2, '#29aab4', '#4f8df2', 'gradient_tan'))
        pl.append(ControlGroupArt(name, 'speed_art', panel, ySpeedKnob-artSpaceAboveKnob, ySpeedKnob+artSpaceBelowKnob, 'gradient_blue'))
        pl.append(ControlGroupArt(name, 'chaos_art', panel, yChaosKnob-artSpaceAboveKnob, yChaosKnob+artSpaceBelowKnob, 'gradient_purple'))
        pl.append(ControlGroupArt(name, 'out_art', panel, outGradY1, outGradY2, 'gradient_tan'))
        controls.append(Component('speed_knob', xmid, ySpeedKnob))
        controls.append(Component('speed_atten', xmid - dxControlGroup, ySpeedKnob + dyControlGroup))
        controls.append(Component('speed_cv', xmid + dxControlGroup, ySpeedKnob + dyControlGroup))
        controls.append(Component('chaos_knob', xmid, yChaosKnob))
        controls.append(Component('chaos_atten', xmid - dxControlGroup, yChaosKnob + dyControlGroup))
        controls.append(Component('chaos_cv', xmid + dxControlGroup, yChaosKnob + dyControlGroup))
        pl.append(ControlTextPath(font, 'SPEED', xmid - 5.5, ySpeedKnob + dyControlText))
        pl.append(ControlTextPath(font, 'CHAOS', xmid - 6.0, yChaosKnob + dyControlText))
        lineGroup = Element('g', 'connector_lines').setAttrib('style', 'fill:none;stroke:#000000;stroke-width:0.25;stroke-linecap:round;stroke-linejoin:bevel;stroke-dasharray:none')
        pl.append(lineGroup)
        lineGroup.append(LineElement(xmid, ySpeedKnob, xmid - dxControlGroup, ySpeedKnob + dyControlGroup))
        lineGroup.append(LineElement(xmid, ySpeedKnob, xmid + dxControlGroup, ySpeedKnob + dyControlGroup))
        lineGroup.append(LineElement(xmid, yChaosKnob, xmid - dxControlGroup, yChaosKnob + dyControlGroup))
        lineGroup.append(LineElement(xmid, yChaosKnob, xmid + dxControlGroup, yChaosKnob + dyControlGroup))
        pl.append(ControlTextPath(font, 'OUT', xOutLabel, yOutLabel, 'out_label'))
        pl.append(ControlTextPath(font, 'X',  xPortLabel, yPortLabel + 0*outputPortDY, 'port_label_x'))
        pl.append(ControlTextPath(font, 'Y',  xPortLabel, yPortLabel + 1*outputPortDY, 'port_label_y'))
        pl.append(ControlTextPath(font, 'Z',  xPortLabel, yPortLabel + 2*outputPortDY, 'port_label_z'))
    return Save(panel, svgFileName)


def GenerateTricorderPanel() -> int:
    PANEL_WIDTH = 25
    svgFileName = '../res/tricorder.svg'
    panel = Panel(PANEL_WIDTH)
    pl = Element('g', 'PanelLayer')
    with Font(SAPPHIRE_FONT_FILENAME) as font:
        pl.append(BorderRect(PANEL_WIDTH, SAPPHIRE_PANEL_COLOR, SAPPHIRE_BORDER_COLOR))
        pl.append(SapphireModelInsignia(panel, font, 'tricorder'))
    panel.append(pl)
    return Save(panel, svgFileName)


def GenerateTinPanel() -> int:
    PANEL_WIDTH = 4
    svgFileName = '../res/tin.svg'
    panel = Panel(PANEL_WIDTH)
    pl = Element('g', 'PanelLayer')
    xmid = panel.mmWidth/2
    inputPortY1 = 30.0
    inputPortDY = 10.0
    xPortLabel = xmid + 5.2
    yPortLabel = inputPortY1 - 2.4
    xInLabel = xmid - 1.9
    yInLabel = inputPortY1 - 10.5
    inGradY1 = yInLabel - 1.5
    inGradY2 = yPortLabel + 2*inputPortDY + 10.0
    controls = ControlLayer()
    defs = Element('defs')
    pl.append(defs)
    defs.append(Gradient(inGradY1, inGradY2, '#a06de4', '#4f8df2', 'gradient_in'))
    with Font(SAPPHIRE_FONT_FILENAME) as font:
        pl.append(BorderRect(PANEL_WIDTH, SAPPHIRE_PANEL_COLOR, SAPPHIRE_BORDER_COLOR))
        pl.append(ModelNamePath(panel, font, 'tin'))
        pl.append(CenteredGemstone(panel))
        pl.append(ControlGroupArt('tin', 'in_art', panel, inGradY1, inGradY2, 'gradient_in'))
        pl.append(ControlTextPath(font, 'X',  xPortLabel, yPortLabel + 0*inputPortDY, 'port_label_x'))
        pl.append(ControlTextPath(font, 'Y',  xPortLabel, yPortLabel + 1*inputPortDY, 'port_label_y'))
        pl.append(ControlTextPath(font, 'Z',  xPortLabel, yPortLabel + 2*inputPortDY, 'port_label_z'))
        controls.append(Component('x_input', xmid, inputPortY1 + 0*inputPortDY))
        controls.append(Component('y_input', xmid, inputPortY1 + 1*inputPortDY))
        controls.append(Component('z_input', xmid, inputPortY1 + 2*inputPortDY))
        pl.append(ControlTextPath(font, 'IN', xInLabel, yInLabel, 'in_label'))
    pl.append(controls)
    panel.append(pl)
    return Save(panel, svgFileName)


def AddControlGroup(pl: Element, controls: ControlLayer, font: Font, symbol: str, label: str, x: float, y: float, dxText: float) -> None:
    dxControlGroup = 5.0
    dyControlGroup = 11.0
    dyControlText = -11.6
    controls.append(Component(symbol + '_knob', x, y))
    controls.append(Component(symbol + '_atten', x - dxControlGroup, y + dyControlGroup))
    controls.append(Component(symbol + '_cv', x + dxControlGroup, y + dyControlGroup))
    pl.append(ControlTextPath(font, label, x - dxText, y + dyControlText))


def AddButton(pl: Element, controls: ControlLayer, font: Font, symbol: str, label: str, x: float, y: float, dxText:float=-6.0, dyText:float=-9.0) -> None:
    controls.append(Component(symbol, x, y))
    pl.append(ControlTextPath(font, label, x + dxText, y + dyText))


def GenerateNucleusPanel() -> int:
    svgFileName = '../res/nucleus.svg'
    PANEL_WIDTH = 16
    panel = Panel(PANEL_WIDTH)
    pl = Element('g', 'PanelLayer')
    panel.append(pl)
    controls = ControlLayer()
    xmid = panel.mmWidth / 2
    dxPort = 12.5               # horizontal distance between X, Y, Z columns.
    yIn = 58.0                  # vertical position of center of input X, Y, Z ports.
    yCushion = 9.0              # vertical space above input ports to put labels ("X", "Y", "Z").
    yOutTop = 84.0              # vertical position of the top row of output ports.
    yOutBottom = 112.0          # vertical position of the bottom row of output ports.
    nOutputParticles = 4        # how many particles are used for output.
    dyOut = (yOutBottom - yOutTop) / (nOutputParticles - 1)     # vertical space between output rows.
    dxVarNameText = 1.0         # half the width of the labels ("X", "Y", "Z"); used for centering.
    yInVarNames = yIn - yCushion  # vertical positions of the labels "X", "Y", "Z".
    yOutVarNames = yOutTop - yCushion
    with open('../src/nucleus_panel.hpp', 'wt') as headerFile:
        headerFile.write('// nucleus_panel.hpp - AUTO-GENERATED; DO NOT EDIT.\n')
        headerFile.write('#pragma once\n')
        headerFile.write('namespace Sapphire\n')
        headerFile.write('{\n')
        headerFile.write('    namespace Nucleus\n')
        headerFile.write('    {\n')
        headerFile.write('        const float PanelDyOut = {:0.16g};\n'.format(dyOut))
        headerFile.write('    }\n')
        headerFile.write('}\n')
    with Font(SAPPHIRE_FONT_FILENAME) as font:
        pl.append(BorderRect(PANEL_WIDTH, SAPPHIRE_PANEL_COLOR, SAPPHIRE_BORDER_COLOR))
        pl.append(ModelNamePath(panel, font, 'nucleus'))
        pl.append(SapphireInsignia(panel, font))
        pl.append(controls)
        xInputCenter = xmid - 12.0
        xPortGridCenter = xmid + 12.0
        xInPos = xInputCenter - dxPort
        xOutPos = xPortGridCenter - dxPort
        for varname in ['x', 'y', 'z']:
            pl.append(ControlTextPath(font, varname.upper(), xInPos - dxVarNameText, yInVarNames))
            pl.append(ControlTextPath(font, varname.upper(), xOutPos - dxVarNameText, yOutVarNames))
            varlabel = Component(varname + '_input', xInPos, yIn)
            controls.append(varlabel)
            ypos = yOutTop
            for i in range(1, 1+nOutputParticles):
                controls.append(Component(varname + str(i) + '_output', xOutPos, ypos))
                ypos += dyOut
            xInPos += dxPort
            xOutPos += dxPort

        yKnobRow1 = 25.0
        yOutLevel = yOutTop + (yOutBottom-yOutTop)/2 + 4.0
        AddControlGroup(pl, controls, font, 'speed', 'SPEED', xmid - 25.0, yKnobRow1, 5.5)
        AddControlGroup(pl, controls, font, 'decay', 'DECAY', xmid, yKnobRow1, 5.5)
        AddControlGroup(pl, controls, font, 'magnet', 'MAGNET', xmid + 25.0, yKnobRow1, 7.0)
        AddControlGroup(pl, controls, font, 'in_drive',  'IN',  xmid + 18.5, yIn - 2.5, 1.5)
        AddControlGroup(pl, controls, font, 'out_level', 'OUT', xmid - 24.0, yOutLevel, 3.5)
        AddButton(pl, controls, font, 'audio_mode_button', 'AUDIO', xmid - 24.0, yOutLevel - 20.0, -5.5)
    return Save(panel, svgFileName)


if __name__ == '__main__':
    sys.exit(
        GenerateChaosPanel('frolic') or
        GenerateChaosPanel('glee') or
        GenerateTricorderPanel() or
        GenerateTinPanel() or
        GenerateNucleusPanel() or
        Print('SUCCESS')
    )
