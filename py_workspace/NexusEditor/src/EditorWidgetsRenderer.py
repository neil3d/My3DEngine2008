import NEG
from NexusEngine import *

class NEWidgetsRenderer(nrender_element):
    def draw(self, PDI):
        # draw x,y,z axis
        PDI.begin_line_batch();
        PDI.draw_line(vector3(0,0,0), vector3(100, 0, 0), color4f(1, 0, 0, 1));
        PDI.draw_line(vector3(0,0,0), vector3(0, 100, 0), color4f(0, 1, 0, 1));
        PDI.draw_line(vector3(0,0,0), vector3(0, 0, 100), color4f(0, 0, 1, 1));
        PDI.end_line_batch();
        
        # draw level editor
        NEG.level_ed.draw_widgets(PDI)