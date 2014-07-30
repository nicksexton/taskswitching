// contains functions for plotting on surfaces (called from in pdpgui_test or other main code)
// Implements basic drawing functions for a cairo drawing area

#include <gtk/gtk.h>
#include <stdlib.h>
#include "lib_cairox.h"
#include "pdpgui_plot.h"


#define DEFAULT_UNIT_SIZE 20
#define DEFAULT_UNIT_PADDING 25

#define AXIS_MARGIN_X 0.1
#define AXIS_MARGIN_Y 0.1
#define AXIS_WIDTH 4
#define AXIS_LABEL_FONT_SIZE 9

#define FONT_HEIGHT 12 // fix with access function



void pdpgui_draw_unit (cairo_t *cr, 
		       PdpguiCoords unit_centre, 
		       PdpguiColourRgb colour_off,
		       PdpguiColourRgb colour_on,
		       double activation) {

  // activation should be real-valued proportion from 0 to 1
  // don't worry about negative activations or resting state for now

  cairo_set_source_rgb (cr, colour_off.r, colour_off.g, colour_off.b);
  cairo_arc (cr, unit_centre.x, unit_centre.y, DEFAULT_UNIT_SIZE, 0, 2*G_PI);
  cairo_fill(cr);

  cairo_set_source_rgb (cr, colour_on.r, colour_on.g, colour_on.b);
  cairo_arc (cr, unit_centre.x, unit_centre.y, DEFAULT_UNIT_SIZE*activation, 0, 2*G_PI);
  cairo_fill(cr);

}

void pdpgui_draw_layer (cairo_t *cr, 
			PdpguiCoords layer_centre, 
			PdpguiColourRgb colour_off,
			PdpguiColourRgb colour_on,
			pdp_layer * layer) {



  int n; 
  for (n = 0; n < layer->size; n ++) {
    
    PdpguiCoords centre = { .x = layer_centre.x 
			    - (layer->size/2) * (DEFAULT_UNIT_SIZE + DEFAULT_UNIT_PADDING) 
			    + n * (DEFAULT_UNIT_SIZE + DEFAULT_UNIT_PADDING),
			    .y = layer_centre.y};

    pdpgui_draw_unit (cr, centre, colour_off, colour_on, layer->units_latest->activations[n]);

  }
  return;

}


void pdpgui_draw_connection (cairo_t *cr, 
			     PdpguiCoords unit_centre_lower, 
			     PdpguiCoords unit_centre_upper) {

  // default black connections
  cairo_set_source_rgb (cr, 0, 0, 0);

  // remember to invert y axis
  cairo_move_to (cr, unit_centre_lower.x, unit_centre_lower.y - DEFAULT_UNIT_SIZE);
  cairo_line_to (cr, unit_centre_upper.x, unit_centre_upper.y + DEFAULT_UNIT_SIZE);
  cairo_stroke(cr);

}



void pdpgui_draw_connection_curved (cairo_t *cr, 
				    PdpguiCoords unit_centre_lower, 
				    PdpguiCoords unit_centre_upper,
				    PdpguiCoords intermediate_lower,
				    PdpguiCoords intermediate_upper) {

  // default black connections
  cairo_set_source_rgb (cr, 0, 0, 0);

  // remember to invert y axis
  cairo_move_to (cr, unit_centre_lower.x, unit_centre_lower.y - DEFAULT_UNIT_SIZE);
  cairo_curve_to (cr,
		  intermediate_lower.x, intermediate_lower.y, 
		  intermediate_upper.x, intermediate_upper.y, 
		  unit_centre_upper.x, unit_centre_upper.y + DEFAULT_UNIT_SIZE);
  cairo_stroke(cr);

}
 
void pdpgui_draw_graph_axes (cairo_t *cr, 
			     guint window_width, guint window_height,
			     int x_divisions, int y_divisions,
			     double x_min, double x_max, 
			     double y_min, double y_max){

  double axis_division_x; 
  double axis_division_y;
  int i;

  double axis_margin_x = AXIS_MARGIN_X * window_width;
  double axis_margin_y = AXIS_MARGIN_Y * window_height;

  PangoLayout *layout;
  char textbuf[32];
  CairoxTextParameters text_params;


  axis_division_x = (window_width - (2 * axis_margin_x)) / x_divisions;
  axis_division_y = (window_height - (2 * axis_margin_y)) / y_divisions;
  printf ("axis margins:\t x: %4.2f\t y: %4.2f\n", axis_margin_x, axis_margin_y);


  // set line parameters
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_set_line_width (cr, 2);

  // set text parameters
  layout = pango_cairo_create_layout (cr);
  pangox_layout_set_font_size (layout, 10);

  // draw x axis
  
  cairo_move_to (cr, axis_margin_x, (window_height - axis_margin_y));
  cairo_line_to (cr, (window_width - axis_margin_x), (window_height - axis_margin_y));
  cairo_stroke(cr);

  // x axis tick marks:
  for (i = 0; i < (x_divisions + 1); i ++) {
    // draw the tick mark
    cairo_move_to (cr, axis_margin_x + (i * axis_division_x), 
		   (window_height - axis_margin_y));
    cairo_line_to (cr, axis_margin_x + (i * axis_division_x), 
		   (window_height - axis_margin_y + (0.01 * window_height)));
    cairo_stroke(cr);

    // draw axis label
    g_snprintf (textbuf, 10, "%3.1f", (x_min + (i * (x_max - x_min) / x_divisions)));     
    cairox_text_parameters_set (&text_params, 
				axis_margin_x + (i * axis_division_x), 
				(window_height - axis_margin_y) + (0.015 * window_height),
				PANGOX_XALIGN_CENTER, PANGOX_YALIGN_TOP, 0.0);
    cairox_paint_pango_text (cr, &text_params, layout, textbuf);

  }
  

    
  // draw y axis
  cairo_move_to (cr, axis_margin_x, axis_margin_y);
  cairo_line_to (cr, axis_margin_x, (window_height - axis_margin_y));
  cairo_stroke(cr);

  // y axis tick marks:
  for (i = 0; i < (y_divisions + 1); i ++) {

    cairo_move_to (cr, axis_margin_x, 
  		   (window_height - axis_margin_y) - (i * axis_division_y));
    cairo_line_to (cr, axis_margin_x - (0.01 * window_width), 
  		   (window_height - axis_margin_y) - (i * axis_division_y));
    cairo_stroke(cr);

    // draw axis label
    g_snprintf (textbuf, 10, "%3.1f", (y_min + (i * (y_max - y_min) / y_divisions)));     
    cairox_text_parameters_set (&text_params, 
				axis_margin_x - (0.015 * window_width), 
				(window_height - axis_margin_y) - (i * axis_division_y),
				PANGOX_XALIGN_RIGHT, PANGOX_YALIGN_CENTER, 0.0);
    cairox_paint_pango_text (cr, &text_params, layout, textbuf);

  }
 
  

  // g_object_unref (layout);
  // cairo_destroy(cr);

}

void pdpgui_plot_vector (cairo_t *cr, 
			 guint window_width, guint window_height,
			 PdpguiAxisDimensions * axis_dimensions,
			 int number_datapoints, double my_data[], 
			 PdpguiColourRgb *plot_colour
			 ){
  double x_offset;
  double y_offset;
  double x_axis_pos = window_width * AXIS_MARGIN_X;
  double y_axis_pos = window_height * (1 - AXIS_MARGIN_Y); 
  double axis_length_x  = window_width * (1 - 2 * AXIS_MARGIN_X);
  double axis_length_y  = window_height * (1 - 2 * AXIS_MARGIN_Y);

  // double x_scale = (axis_dimensions->x_max - axis_dimensions->x_min) / axis_length_x;
  double y_scale = (axis_dimensions->y_max - axis_dimensions->y_min) / axis_length_y;

  // set line parameters
  cairo_set_dash(cr, NULL, 0, 0); // dashed line off
  cairo_set_source_rgb (cr, plot_colour->r, plot_colour->g, plot_colour->b);
  cairo_set_line_width (cr, 2);


  // first data point
  x_offset = 0;
  y_offset = (my_data[0] - axis_dimensions->y_min )/ y_scale;
  
  cairo_move_to (cr, 
		 x_axis_pos, // x = 0
		 y_axis_pos - y_offset);

  int n;
  for (n = 0; n < number_datapoints; n ++) {
    
    x_offset = ((double) n / number_datapoints) * axis_length_x;
    y_offset = (my_data[n] - axis_dimensions->y_min) / y_scale;

    cairo_line_to (cr, 
		   x_axis_pos + x_offset, 
		   y_axis_pos - y_offset);
  }
  cairo_stroke (cr);
		 
}

void pdpgui_plot_vector_dashed (cairo_t *cr, 
				guint window_width, guint window_height,
				PdpguiAxisDimensions * axis_dimensions,
				int number_datapoints, double my_data[], 
				PdpguiColourRgb *plot_colour
				){
  double x_offset;
  double y_offset;
  double x_axis_pos = window_width * AXIS_MARGIN_X;
  double y_axis_pos = window_height * (1 - AXIS_MARGIN_Y); 
  double axis_length_x  = window_width * (1 - 2 * AXIS_MARGIN_X);
  double axis_length_y  = window_height * (1 - 2 * AXIS_MARGIN_Y);

  // double x_scale = (axis_dimensions->x_max - axis_dimensions->x_min) / axis_length_x;
  double y_scale = (axis_dimensions->y_max - axis_dimensions->y_min) / axis_length_y;

  // set line parameters
  cairo_set_source_rgb (cr, plot_colour->r, plot_colour->g, plot_colour->b);
  cairo_set_line_width (cr, 2);

  double dash_pattern[2] = {5, 5};
  cairo_set_dash(cr, dash_pattern, 2, 0);


  // first data point
  x_offset = 0;
  y_offset = (my_data[0] - axis_dimensions->y_min )/ y_scale;
  
  cairo_move_to (cr, 
		 x_axis_pos, // x = 0
		 y_axis_pos - y_offset);

  int n;
  for (n = 0; n < number_datapoints; n ++) {
    
    x_offset = ((double) n / number_datapoints) * axis_length_x;
    y_offset = (my_data[n] - axis_dimensions->y_min) / y_scale;

    cairo_line_to (cr, 
		   x_axis_pos + x_offset, 
		   y_axis_pos - y_offset);
  }
  cairo_stroke (cr);
		 
}

		 




