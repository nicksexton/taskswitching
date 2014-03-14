#ifndef pdpgui_plot_h
#define pdpgui_plot_h

#include <gtk/gtk.h>

typedef struct pdpgui_coords {
  double x; 
  double y; 
} PdpguiCoords;

typedef struct pdpgui_colour_rgb {
  double r; 
  double g; 
  double b;
} PdpguiColourRgb;

typedef struct pdpgui_axis_dimensions {
  double x_min;
  double x_max;
  double y_min;
  double y_max;
} PdpguiAxisDimensions;


void pdpgui_draw_unit (cairo_t *cr, 
		       PdpguiCoords unit_centre, 
		       PdpguiColourRgb colour_off,
		       PdpguiColourRgb colour_on,
		       double activation);


void pdpgui_draw_connection (cairo_t *cr, 
			     PdpguiCoords unit_centre_lower, 
			     PdpguiCoords unit_centre_upper);

void pdpgui_draw_connection_curved (cairo_t *cr, 
				    PdpguiCoords unit_centre_lower, 
				    PdpguiCoords unit_centre_upper,
				    PdpguiCoords intermediate_lower,
				    PdpguiCoords intermediate_upper);



void pdpgui_draw_graph_axes (cairo_t *cr, 
			     guint window_width, guint window_height,
			     int x_divisions, int y_divisions,
			     double x_min, double x_max, 
			     double y_min, double y_max);

// plots my_data as y values, equally spaced along the x axis
void pdpgui_plot_vector (cairo_t *cr, 
			 guint window_width, guint window_height,
			 PdpguiAxisDimensions * axis_dimensions,
			 int number_datapoints, double my_data[], 
			 PdpguiColourRgb *plot_colour
			 );



#endif
