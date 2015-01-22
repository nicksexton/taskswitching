#include <ctype.h> // for isprint; processing cmd line arguments
#include <gtk/gtk.h>
#include "pdp_objects.h"
#include "3task_import.h"
#include "3task_model_gs.h" // model
#include "3task_procedure.h"
#include "lib_cairox.h" // for pango annotations

#include "pdpgui_plot.h"
// #include "pdpgui_import.h"
#include "3task_gui_koch_conflict.h"
#include "3task_model_koch.h"

#define DATAFILE "3task_koch_data_.txt"
#define DATAFILE_ACT "3task_koch_act.txt"

#define TEXT_SIZE_HEAD 15
#define EXPORT_IMAGE_WIDTH 800
#define EXPORT_IMAGE_HEIGHT 500



void three_task_gui_koch_conflict_plot_conflict_activation (GtkWidget *widget, 
							   cairo_t *cr, 
							   ThreeTaskSimulation *simulation) {

  guint widget_width, widget_height;
  widget_width = gtk_widget_get_allocated_width (GTK_WIDGET(widget));
  widget_height = gtk_widget_get_allocated_height (GTK_WIDGET(widget));

  PdpguiAxisDimensions axes = { 
    .x_min = 0.0, 
    .x_max = simulation->model->cycle * 1.0, 
    .y_min = -1.0, 
    .y_max = 1.0
  };

  pdpgui_draw_graph_axes(cr, widget_width, widget_height, 10, 10, 
			 axes.x_min, axes.x_max, 
			 axes.y_min, axes.y_max);





  PdpguiColourRgb plot_conflict[3] = {
    {.r = 0.7, .g = 0.7, .b = 0.0}, // yellow - ie. units 0 and 1
    {.r = 0.0, .g = 0.7, .b = 0.7}, // cyan - ie units 1 and 2
    {.r = 0.7, .g = 0.0, .b = 0.7}, // magenta - ie units 2 and 0
  };



  // now construct an arbitrary vector;
  double * units_activation; // check this doesn't overflow??

    // ---------------------- PLOT CONFLICT MONITORING UNITS ---------------------

  pdp_layer * conflict = pdp_model_component_find (simulation->model, 
							ID_CONFLICT)->layer; 

  pdp_layer * conflict_input = pdp_model_component_find (simulation->model, 
							ID_CONFLICT_INPUT)->layer; 

  int unit;
  for (unit = 0; unit < 3; unit ++) {

    // plot main conflict units  
    units_activation = 
      pdp_layer_get_unit_activation_history (conflict, unit, simulation->model->cycle);
    
    if (units_activation == NULL) {
      free(units_activation);
    }
    else {
      pdpgui_plot_vector (cr, widget_width, widget_height, &axes, 
			  simulation->model->cycle, units_activation, 
			  &(plot_conflict[unit]));
      free(units_activation);
      units_activation = NULL;
    }

    // temp - plot input to conflict units

    printf ("conflict input units_initial activation 0 = %4.2f\n", conflict_input->units_initial.activations[0]);
    units_activation = 
      pdp_layer_get_unit_activation_history (conflict_input, unit, simulation->model->cycle);
    
    if (units_activation == NULL) {
      printf ("three_task_gui_koch_conflict_plot_conflict_activation could not get conflict_input units\n"); 
      free(units_activation);
    }
    else {
      pdpgui_plot_vector_dashed (cr, widget_width, widget_height, &axes, 
				 simulation->model->cycle, units_activation, 
				 &(plot_conflict[unit]));
      free(units_activation);
    }

  }




}

void three_task_gui_koch_conflict_plot_network_activation (GtkWidget *widget, 
							   cairo_t *cr, 
							   ThreeTaskSimulation *simulation) {


  guint widget_width, widget_height;
  widget_width = gtk_widget_get_allocated_width (GTK_WIDGET(widget));
  widget_height = gtk_widget_get_allocated_height (GTK_WIDGET(widget));

  // printf ("%d x %d\n", widget_width, widget_height);

  // Draw main graph

  pdpgui_draw_graph_axes(cr, widget_width, widget_height, 10, 10, 
			 0.0, simulation->model->cycle * 1.0, 
			 -1.0, 1.0);



  PdpguiAxisDimensions axes = { 
    .x_min = 0.0, 
    .x_max = simulation->model->cycle * 1.0, 
    .y_min = -1.0, 
    .y_max = 1.0
  };

  PdpguiColourRgb plot_red[2] = {{ .r = 1.0, .g = 0.0, .b = 0.0 }, { .r = 1.0, .g = 0.4, .b = 0.4 }};
  PdpguiColourRgb plot_green[2] = {{ .r = 0.0, .g = 0.6, .b = 0.0 }, { .r = 0.3, .g = 0.6, .b = 0.3 }};
  PdpguiColourRgb plot_blue[2] = {{ .r = 0.0, .g = 0.0, .b = 1.0 }, { .r = 0.4, .g = 0.4, .b = 1.0 }};



  PdpguiColourRgb plot_td[3] = {
    { .r = 1.0, .g = 0.0, .b = 0.0 }, 
    { .r = 0.0, .g = 0.6, .b = 0.0 },
    { .r = 0.0, .g = 0.0, .b = 1.0 },
  };



  // now construct an arbitrary vector;
  double * units_activation; // check this doesn't overflow??


  // <------------ OUTPUT 0 ------------
  pdp_layer * current_layer = pdp_model_component_find (simulation->model, 
							ID_OUTPUT_0)->layer; 
  int unit;


  for (unit = 0; unit < current_layer->size; unit ++) {

    units_activation = 
      pdp_layer_get_unit_activation_history (current_layer, unit, simulation->model->cycle);
  
    if (units_activation == NULL) {
      free(units_activation);
      break;
    }
    else {
      pdpgui_plot_vector (cr, widget_width, widget_height, &axes, 
			simulation->model->cycle, units_activation, &(plot_red[unit]));

      // want more sophisticated rendering algorithm using buffering, 
      // this will probably cause flicker

      free(units_activation);
    }
  }


  // <------------ OUTPUT 1 ------------
  current_layer = pdp_model_component_find (simulation->model, 
							ID_OUTPUT_1)->layer; 


  for (unit = 0; unit < current_layer->size; unit ++) {

    units_activation = 
      pdp_layer_get_unit_activation_history (current_layer, unit, simulation->model->cycle);
  
    if (units_activation == NULL) {
      free(units_activation);
      break;
    }
    else {
      pdpgui_plot_vector (cr, widget_width, widget_height, &axes, 
			simulation->model->cycle, units_activation, &(plot_green[unit]));

      // want more sophisticated rendering algorithm using buffering, 
      // this will probably cause flicker

      free(units_activation);
    }
  }

  // <------------ OUTPUT 1 ------------
  current_layer = pdp_model_component_find (simulation->model, 
							ID_OUTPUT_2)->layer; 


  for (unit = 0; unit < current_layer->size; unit ++) {

    units_activation = 
      pdp_layer_get_unit_activation_history (current_layer, unit, simulation->model->cycle);
  
    if (units_activation == NULL) {
      free(units_activation);
      break;
    }
    else {
      pdpgui_plot_vector (cr, widget_width, widget_height, &axes, 
			  simulation->model->cycle, units_activation, &(plot_blue[unit]));

      // want more sophisticated rendering algorithm using buffering, 
      // this will probably cause flicker

      free(units_activation);
    }
  }




  // <------------ NEXT PLOT TASKDEMAND UNITS  ------------------

  current_layer = pdp_model_component_find (simulation->model, 
							ID_TASKDEMAND)->layer; 

  //  task demand unit 1
  units_activation = 
    pdp_layer_get_unit_activation_history (current_layer, 0, simulation->model->cycle);
  
  if (units_activation == NULL) {
    free(units_activation);
  }
  else {
    pdpgui_plot_vector_dashed (cr, widget_width, widget_height, &axes, 
		      simulation->model->cycle, units_activation, 
		      &(plot_td[0]));
  }

    // task demand unit 2
  units_activation = 
    pdp_layer_get_unit_activation_history (current_layer, 1, simulation->model->cycle);
  
    if (units_activation == NULL) {
      free(units_activation);
    }
    else {
      pdpgui_plot_vector_dashed (cr, widget_width, widget_height, &axes, 
				 simulation->model->cycle, units_activation, 
				 &(plot_td[1]));
      free(units_activation);
    }

    // task demand unit 3
  units_activation = 
    pdp_layer_get_unit_activation_history (current_layer, 2, simulation->model->cycle);
  
    if (units_activation == NULL) {
      free(units_activation);
    }
    else {
      pdpgui_plot_vector_dashed (cr, widget_width, widget_height, &axes, 
				 simulation->model->cycle, units_activation, 
				 &(plot_td[2]));
      free(units_activation);
    }

  
}



/******************************************************************************/


void draw_architecture (cairo_t *cr, int width, int height, ThreeTaskSimulation *simulation) {


  pdp_layer * layer_taskdemand = pdp_model_component_find (simulation->model, ID_TASKDEMAND)->layer;
  pdp_layer * layer_input_0 = pdp_model_component_find (simulation->model, ID_INPUT_0)->layer;
  pdp_layer * layer_input_1 = pdp_model_component_find (simulation->model, ID_INPUT_1)->layer;
  pdp_layer * layer_input_2 = pdp_model_component_find (simulation->model, ID_INPUT_2)->layer;
  pdp_layer * layer_output_0 = pdp_model_component_find (simulation->model, ID_OUTPUT_0)->layer;
  pdp_layer * layer_output_1 = pdp_model_component_find (simulation->model, ID_OUTPUT_1)->layer;
  pdp_layer * layer_output_2 = pdp_model_component_find (simulation->model, ID_OUTPUT_2)->layer;
  pdp_layer * layer_topdowncontrol = pdp_model_component_find (simulation->model, ID_TOPDOWNCONTROL)->layer;
  pdp_layer * layer_conflict = pdp_model_component_find (simulation->model, ID_CONFLICT)->layer;
  //  pdp_layer * layer_conflict_input = pdp_model_component_find (simulation->model, ID_CONFLICT_INPUT)->layer;


  PdpguiCoords loc_conflict = { .x = width * 0.475, .y = height * 0.1, };
  PdpguiCoords loc_conflict_title = { .x = width * 0.9, .y = height * 0.1, };
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD, loc_conflict_title, 0, -10, "Conflict Monitoring");
  //  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_taskdemand, -70, -75, "A");
  //  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_taskdemand, -21, -75, "B");
  //  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_taskdemand, 21, -75, "C");

  PdpguiCoords loc_conflict_td_upper = { .x = width * 0.05, .y = height * -0.2 };
  PdpguiCoords loc_conflict_td_lower = { .x = width * 0.05, .y = height * 0.8 };


  // Draw a dashed line across the screen to separate new model content
  PdpguiCoords dashed_separator_left = { .x = width * 0.05, .y = height * 0.2 };
  PdpguiCoords dashed_separator_right = { .x = width * 0.95, .y = height * 0.2 };
  cairo_set_source_rgb (cr, 0.5, 0.5, 0.5);
  cairo_set_line_width (cr, 3);
  double dash_pattern[2] = {5, 5};
  cairo_set_dash(cr, dash_pattern, 2, 0); 
  cairo_move_to (cr, dashed_separator_right.x, dashed_separator_right.y);
  cairo_line_to (cr, dashed_separator_left.x, dashed_separator_left.y);
  cairo_stroke(cr);  

  // Conflict Inputs
  //  PdpguiCoords loc_conflict_input = { .x = width * 0.7, .y = height * 0.2, };
  //  PdpguiCoords loc_conflict_input_title = { .x = width * 0.9, .y = height * 0.2, };
  //  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD, loc_conflict_input_title, 0, -10, "Conflict (Input)");


  // Now draw the main units
  // TD Units

  PdpguiCoords loc_taskdemand = { .x = width * 0.45, .y = height * 0.4, };
  PdpguiCoords loc_taskdemand_title = { .x = width * 0.9, .y = height * 0.4, };
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD, loc_taskdemand_title, 0, -10, "Task Demand");
  //  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_taskdemand, -70, -75, "A");
  //  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_taskdemand, -21, -75, "B");
  //  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_taskdemand, 21, -75, "C");



  // Inputs

  PdpguiCoords loc_input_0 = { .x = width * 0.2, .y = height * 0.85, };
  PdpguiCoords loc_input_1 = { .x = width * 0.65, .y = height * 0.85, };
  PdpguiCoords loc_input_2 = { .x = width * 0.8, .y = height * 0.85, };
  PdpguiCoords loc_inputs_title = { .x = width * 0.9, .y = height * 0.85, };
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD, loc_inputs_title, 0, -10, "Inputs");
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_input_0, -22, 30, "A");
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_input_1, -22, 30, "B");
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_input_2, -22, 30, "C");


  // Outputs

  PdpguiCoords loc_output_0 = { .x = width * 0.2, .y = height * 0.7, };
  PdpguiCoords loc_output_1 = { .x = width * 0.65, .y = height * 0.7, };
  PdpguiCoords loc_output_2 = { .x = width * 0.8, .y = height * 0.7, };
  PdpguiCoords loc_outputs_title = { .x = width * 0.9, .y = height * 0.7, };
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD, loc_outputs_title, 0, -10, "Outputs");


  
  PdpguiCoords loc_topdowncontrol = { .x = width * 0.45, .y = height * 0.80, };
  PdpguiCoords loc_tdc_title = { .x = width * 0.45, .y = height * 0.88, };
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD, loc_tdc_title, -30, -10, "Top Down");
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD, loc_tdc_title, -25, 12, "Control");
  

  PdpguiCoords loc_td_input0_intermed_upper = { .x = width * 0.05, .y = 0.0 };
  PdpguiCoords loc_td_input0_intermed_lower = { .x = width * 0.05, .y = height * 0.95 };
  PdpguiCoords loc_td_input1_intermed_upper = { .x = width * 0.85, .y = height * 0.20 };
  PdpguiCoords loc_td_input1_intermed_lower = { .x = width * 0.70, .y = height * 0.90 };
  PdpguiCoords loc_td_input2_intermed_upper = { .x = width * 0.9, .y = 0.0 };
  PdpguiCoords loc_td_input2_intermed_lower = { .x = width * 0.9, .y = height * 0.90 };

  //  PdpguiCoords loc_channels_title = { .x = width * 0.5, .y = height * 0.95, };
  //  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD, loc_channels_title, -50, 0, "Task Processing Pathways");

  // intermediates for lateral connections
  PdpguiCoords loc_outputs_lateral_intermed_upper = { .x = width * 0.5, 
						      .y = height * 0.3 };
  PdpguiCoords loc_outputs_lateral_intermed_lower = { .x = width * 0.5, 
						.y = height * 0.70 };

  PdpguiCoords loc_td_lateral_intermed_upper = { .x = width * 0.4, 
						      .y = height * 0.1 };
  PdpguiCoords loc_td_lateral_intermed_lower = { .x = width * 0.4, 
						.y = height * 0.40 };


  PdpguiColourRgb mono[2] = {{ 
      .r = 0.1, 
      .g = 0.1, 
      .b = 0.1 
    }, {
      .r = 0.7, 
      .g = 0.1, 
      .b = 0.1 
    }};

  PdpguiColourRgb mono_grey[2] = {{ 
      .r = 0.4, 
      .g = 0.4, 
      .b = 0.4 
    }, {
      .r = 0.6, 
      .g = 0.6, 
      .b = 0.6 
    }};


  pdpgui_draw_layer (cr, loc_taskdemand, mono[0], mono[1], layer_taskdemand);
  pdpgui_draw_layer (cr, loc_input_0, mono[0], mono[1], layer_input_0);
  pdpgui_draw_layer (cr, loc_input_1, mono[0], mono[1], layer_input_1);
  pdpgui_draw_layer (cr, loc_input_2, mono[0], mono[1], layer_input_2);
  pdpgui_draw_layer (cr, loc_output_0, mono[0], mono[1], layer_output_0);
  pdpgui_draw_layer (cr, loc_output_1, mono[0], mono[1], layer_output_1);
  pdpgui_draw_layer (cr, loc_output_2, mono[0], mono[1], layer_output_2);
  pdpgui_draw_layer (cr, loc_topdowncontrol, mono[0], mono[1], layer_topdowncontrol);
  pdpgui_draw_layer (cr, loc_conflict, mono[0], mono[1], layer_conflict); 

  // why mono grey?
  //  pdpgui_draw_layer (cr, loc_topdowncontrol, mono_grey[0], mono_grey[1], layer_topdowncontrol);
  //  pdpgui_draw_layer (cr, loc_conflict, mono_grey[0], mono_grey[1], layer_conflict);
  //  pdpgui_draw_layer (cr, loc_conflict_input, mono_grey[0], mono_grey[1], layer_conflict_input);

  // overlay task demand annotations on units
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_taskdemand, -70, -15, "A");
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_taskdemand, -21, -15, "B");
  pdpgui_pango_print_annotation (cr, TEXT_SIZE_HEAD + 10, loc_taskdemand, 21, -15, "C");


  pdpgui_draw_weights (cr, loc_topdowncontrol, loc_taskdemand, 
		       pdp_input_find(layer_taskdemand, ID_TOPDOWNCONTROL)->input_weights);

  pdpgui_draw_weights (cr, loc_input_0, loc_output_0, pdp_input_find(layer_output_0, ID_INPUT_0)->input_weights);
  pdpgui_draw_weights (cr, loc_input_1, loc_output_1, pdp_input_find(layer_output_1, ID_INPUT_1)->input_weights);
  pdpgui_draw_weights (cr, loc_input_2, loc_output_2, pdp_input_find(layer_output_2, ID_INPUT_2)->input_weights);

  pdpgui_draw_weights (cr, loc_input_0, loc_taskdemand, pdp_input_find(layer_taskdemand, ID_INPUT_0)->input_weights);
  pdpgui_draw_weights (cr, loc_input_1, loc_taskdemand, pdp_input_find(layer_taskdemand, ID_INPUT_1)->input_weights);
  pdpgui_draw_weights (cr, loc_input_2, loc_taskdemand, pdp_input_find(layer_taskdemand, ID_INPUT_2)->input_weights);

  pdpgui_draw_weights (cr, loc_output_0, loc_taskdemand, 
		       pdp_input_find(layer_taskdemand, ID_OUTPUT_0)->input_weights);
  pdpgui_draw_weights (cr, loc_output_1, loc_taskdemand, 
		       pdp_input_find(layer_taskdemand, ID_OUTPUT_1)->input_weights);
  pdpgui_draw_weights (cr, loc_output_2, loc_taskdemand, 
		       pdp_input_find(layer_taskdemand, ID_OUTPUT_2)->input_weights);

  pdpgui_draw_weights_topdown (cr, loc_taskdemand, loc_output_0, 
			       loc_td_input0_intermed_upper, loc_td_input0_intermed_lower,
			       pdp_input_find(layer_output_0, ID_TASKDEMAND)->input_weights);
  pdpgui_draw_weights_topdown (cr, loc_taskdemand, loc_output_1, 
			       loc_td_input1_intermed_upper, loc_td_input1_intermed_lower,
			       pdp_input_find(layer_output_1, ID_TASKDEMAND)->input_weights);
  pdpgui_draw_weights_topdown (cr, loc_taskdemand, loc_output_2, 
			       loc_td_input2_intermed_upper, loc_td_input2_intermed_lower,
			       pdp_input_find(layer_output_2, ID_TASKDEMAND)->input_weights);


  // conflict monitoring weights
  pdpgui_draw_weights_topdown (cr, loc_conflict, loc_taskdemand, 
			       loc_conflict_td_upper, loc_conflict_td_lower,
			       pdp_input_find(layer_taskdemand, ID_CONFLICT)->input_weights);

  //  pdpgui_draw_weights (cr, loc_conflict_input, loc_conflict, 
  //		       pdp_input_find(layer_conflict, ID_CONFLICT_INPUT)->input_weights);


  // Now draw (imaginary) weights for TD to conflict units:
  double td_conflict_weights_matrix[3][3] = {
    {2.0, 2.0, 0.0},
    {0.0, 2.0, 2.0},
    {2.0, 0.0, 2.0},
  };
  pdp_weights_matrix * td_conflict_weights = pdp_weights_create (3,3);
  pdp_weights_set (td_conflict_weights, 3, 3, td_conflict_weights_matrix);
  pdpgui_draw_weights (cr, loc_taskdemand, loc_conflict, td_conflict_weights);
  pdp_weights_free (td_conflict_weights);


  /* Debug only
  // draw weights for lateral connections
  pdpgui_draw_weights_topdown (cr, loc_output_0, loc_output_1, 
			       loc_outputs_lateral_intermed_upper, 
			       loc_outputs_lateral_intermed_lower,
			       pdp_input_find(layer_output_1, ID_OUTPUT_0)->input_weights);

  pdpgui_draw_weights_topdown (cr, loc_output_0, loc_output_2, 
			       loc_outputs_lateral_intermed_upper, 
			       loc_outputs_lateral_intermed_lower,
			       pdp_input_find(layer_output_2, ID_OUTPUT_0)->input_weights);

  pdpgui_draw_weights_topdown (cr, loc_output_1, loc_output_0, 
			       loc_outputs_lateral_intermed_upper, 
			       loc_outputs_lateral_intermed_lower,
			       pdp_input_find(layer_output_0, ID_OUTPUT_1)->input_weights);

  pdpgui_draw_weights_topdown (cr, loc_output_1, loc_output_2, 
			       loc_outputs_lateral_intermed_upper, 
			       loc_outputs_lateral_intermed_lower,
			       pdp_input_find(layer_output_2, ID_OUTPUT_1)->input_weights);

  pdpgui_draw_weights_topdown (cr, loc_output_2, loc_output_0, 
			       loc_outputs_lateral_intermed_upper, 
			       loc_outputs_lateral_intermed_lower,
			       pdp_input_find(layer_output_0, ID_OUTPUT_2)->input_weights);

  pdpgui_draw_weights_topdown (cr, loc_output_2, loc_output_1, 
			       loc_outputs_lateral_intermed_upper, 
			       loc_outputs_lateral_intermed_lower,
			       pdp_input_find(layer_output_1, ID_OUTPUT_2)->input_weights);


  // within-module inhibitory connections

  pdpgui_draw_weights_topdown (cr, loc_taskdemand, loc_taskdemand, 
			       loc_td_lateral_intermed_upper, 
			       loc_td_lateral_intermed_lower,
			       pdp_input_find(layer_taskdemand, ID_TASKDEMAND)->input_weights);

  //  pdpgui_draw_weights (cr, loc_topdowncontrol, loc_taskdemand, 
  //                       pdp_input_find(layer_taskdemand, ID_TOPDOWNCONTROL)->input_weights);
  */


}

void three_task_gui_draw_architecture (GtkWidget *widget, 
				       cairo_t *cr, 
				       ThreeTaskSimulation *simulation) {


  guint widget_width, widget_height;
  widget_width = gtk_widget_get_allocated_width (GTK_WIDGET(widget));
  widget_height = gtk_widget_get_allocated_height (GTK_WIDGET(widget));

  //  printf ("%d x %d\n", widget_width, widget_height);

  // Conflict Units
  draw_architecture (cr, widget_width, widget_height, simulation);


  return;

}


static GtkWidget* 
create_sub_notepage_model_plot_activation (ThreeTaskObjects * objects) {
  // plots network activation for current trial


  GtkWidget *grid;
  GtkWidget *drawing_area_main, *drawing_area_conflict;
  GtkWidget *label;

  label = gtk_label_new("Network activation graph here");

  drawing_area_main = gtk_drawing_area_new();
  g_signal_connect (drawing_area_main, "draw", 
		    G_CALLBACK(three_task_gui_koch_conflict_plot_network_activation), objects->simulation);
  gtk_widget_set_hexpand (drawing_area_main, TRUE);
  gtk_widget_set_vexpand (drawing_area_main, TRUE);

  drawing_area_conflict = gtk_drawing_area_new();
  g_signal_connect (drawing_area_conflict, "draw", 
		    G_CALLBACK(three_task_gui_koch_conflict_plot_conflict_activation), objects->simulation);
  gtk_widget_set_hexpand (drawing_area_conflict, TRUE);
  gtk_widget_set_vexpand (drawing_area_conflict, TRUE);


  grid = gtk_grid_new();
  gtk_grid_attach (GTK_GRID(grid), label, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID(grid), drawing_area_main, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID(grid), drawing_area_conflict, 0, 2, 1, 1);

  gtk_widget_show_all(grid);


  return (grid);
}


static void print_architecture(GtkWidget *caller, ThreeTaskObjects * objects)
{

    cairo_surface_t *surface;
    cairo_t *cr;



    surface = cairo_pdf_surface_create("screen_dump_architecture.pdf", EXPORT_IMAGE_WIDTH, EXPORT_IMAGE_HEIGHT);
    cr = cairo_create(surface);
    draw_architecture (cr, EXPORT_IMAGE_WIDTH, EXPORT_IMAGE_HEIGHT, objects->simulation);
    
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

}




static GtkWidget* 
create_sub_notepage_model_display_architecture (ThreeTaskObjects * objects) {

  GtkWidget *toolbar;
  GtkToolItem *tool_item;
  GtkWidget *drawing_area;
  GtkWidget *grid;
  //  GtkWidget *label;

  // todo - add a widget to print the architecture

  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

  // Toolbar (could be better use of space?)
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_SAVE);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
		    G_CALLBACK(print_architecture), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Save Archiecture as pdf/png");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, 0);


  // Drawing area
  drawing_area = gtk_drawing_area_new();
  g_signal_connect (drawing_area, "draw", 
		    G_CALLBACK(three_task_gui_draw_architecture), objects->simulation);
  gtk_widget_set_size_request (drawing_area, EXPORT_IMAGE_WIDTH, EXPORT_IMAGE_HEIGHT);
  gtk_widget_set_hexpand (drawing_area, TRUE);
  gtk_widget_set_vexpand (drawing_area, TRUE);



  grid = gtk_grid_new();
  gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(toolbar), 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID(grid), drawing_area, 0, 1, 1, 1);
  //  gtk_grid_attach (GTK_GRID(grid), label, 0, 0, 1, 1);

  gtk_widget_show_all(grid);
  return (grid);

}

static void model_set_starting_activation (ThreeTaskObjects * objects) {

  ThreeTaskSimulation * simulation = objects->simulation;


  double input_0_initial_act[2]   = { 0.0,  0.0 };
  double input_1_initial_act[2]   = { 0.0,  0.0 };
  double input_2_initial_act[2]   = { 0.0,  0.0 };
  double topdown_control_initial_act[3]   = { 0.0,  0.0, 0.0 };

  GtkTreeIter *iter = g_malloc (sizeof(GtkTreeIter));

  if (!gtk_tree_model_get_iter(GTK_TREE_MODEL(simulation->task_store), iter, simulation->current_trial_path)) {

    g_free (iter);
    printf ("error! three_task_model_dummy_run  failed to acquire valid iter from current_trial_path,"
	    "returning FALSE\n");
    return;
  }

  else {

    // set appropriate inputs based on trial data 
    int trial_id, cue, stim_0, stim_1, stim_2;
    gtk_tree_model_get (GTK_TREE_MODEL(simulation->task_store), iter, 
			COL_TASK_ID, &trial_id,
			COL_TASK_PATTERN_1, &cue, 
			COL_TASK_PATTERN_2, &stim_0,
			COL_TASK_PATTERN_3, &stim_1,
			COL_TASK_PATTERN_4, &stim_2,
			-1);

  // check that trial parameters are sensible

  if (stim_0 < -1 || stim_0 > 2) {
    printf ("subject data: input_0 %d out of range (should be -1 (neutral) or 0 - 1)!",
	    stim_0);
    return ;
  }

  if (stim_1 < -1 || stim_1 > 2) {
    printf ("subject data: input_1 %d out of range (should be -1 (neutral) or 0 - 1)!",
	    stim_1);
    return ;
  }

  if (stim_2 < -1 || stim_2 > 2) {
    printf ("subject data: input_2 %d out of range (should be -1 (neutral) or 0 - 1)!",
	    stim_2);
    return ;
  }

  if (cue < 0 || cue > 3) {
    printf ("subject data: task input %d out of range (should be 0 - 2)!",
	    cue);
    return ;
  }


  // set ON inputs
  if (stim_0 >= 0) { // check for neutral trial condition where stim is -1
      input_0_initial_act[stim_0] = 1.0;
  }

  if (stim_1 >= 0) { // check for neutral trial condition where stim is -1
      input_1_initial_act[stim_1] = 1.0;
  }

  if (stim_2 >= 0) { // check for neutral trial condition where stim is -1
      input_2_initial_act[stim_2] = 1.0;
  }

  topdown_control_initial_act[cue] = 1.0;

  pdp_layer_set_activation (pdp_model_component_find (simulation->model, ID_INPUT_0)->layer, 
			    2, input_0_initial_act);
  pdp_layer_set_activation (pdp_model_component_find (simulation->model, ID_INPUT_1)->layer, 
			    2, input_1_initial_act);
  pdp_layer_set_activation (pdp_model_component_find (simulation->model, ID_INPUT_2)->layer, 
			    2, input_2_initial_act);
  pdp_layer_set_activation (pdp_model_component_find (
                                 simulation->model, ID_TOPDOWNCONTROL)->layer, 
	   		    3, topdown_control_initial_act);

  if (objects->model_sub_notepage != NULL) {
    gtk_widget_queue_draw(objects->model_sub_notepage);
  }

  }
}


static void model_controls_initialise_cb (GtkToolItem * tool_item, 
					  ThreeTaskObjects * objects) {

  ThreeTaskSimulation * simulation = objects->simulation;

  procedure_change_trial_first (simulation, simulation->task_store);
  objects->model_reinit (simulation->model, INITIAL, simulation);
  //  three_task_model_dummy_reinit (simulation->model, INITIAL, simulation); 

  model_set_starting_activation (objects);

  GtkTreeSelection *sel;
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(objects->task_tree_view));
  gtk_tree_selection_select_path(sel, objects->simulation->current_trial_path);

}


static void model_controls_step_once_cb (GtkToolItem * tool_item, 
					 ThreeTaskObjects * objects) {

  double response_threshold = *(double *)g_hash_table_lookup(objects->simulation->model_params_htable, 
							     "response_threshold");
  
  // check what to do (if anything) about negative conflict
  conflict_negative_options conflict_negative = 
    *(gint *)g_hash_table_lookup(objects->simulation->model_params_htable, "conflict_negative");  

  if (!stopping_condition (objects->simulation->model, response_threshold)) { 
    gchar * path;
    path = gtk_tree_path_to_string(objects->simulation->current_trial_path);


    three_task_model_koch_conflict_run_step (objects->simulation->model, objects->simulation->random_generator, 
					     response_threshold, 
					     objects->simulation->datafile,
					     objects->simulation->datafile_act,
					     path,
					     conflict_negative);

    g_free (path);
    if (objects->model_sub_notepage != NULL) {
      gtk_widget_queue_draw(objects->model_sub_notepage);
    }
  }
 
}


static void model_controls_run_cb (GtkToolItem * tool_item, 
				   ThreeTaskObjects * objects) {

  objects->model_run (objects->simulation->model, objects->simulation);

  if (objects->model_sub_notepage != NULL) {
    gtk_widget_queue_draw(objects->model_sub_notepage);
  }


}


static void model_controls_continue_cb (GtkToolItem * tool_item, 
					ThreeTaskObjects * objects) {

  double response_threshold = *(double *)g_hash_table_lookup(objects->simulation->model_params_htable, 
							     "response_threshold");
  //  double learning_rate = *(double *)g_hash_table_lookup(objects->simulation->model_params_htable, "learning_rate");
  // int hebb_persist = *(int *)g_hash_table_lookup(objects->simulation->model_params_htable, 
  //					     "hebb_persist");


  while (!stopping_condition(objects->simulation->model, response_threshold)) {
      objects->model_run (objects->simulation->model, objects->simulation);
  }

  //   objects->model_reinit(objects->simulation->model, TRIAL, objects->simulation);


  if (!procedure_current_trial_is_last(objects->simulation)) {
    procedure_change_trial_next (objects->simulation);
    objects->model_reinit(objects->simulation->model, TRIAL, objects->simulation);
  }
  else if (!procedure_current_block_is_last(objects->simulation)) {
    procedure_change_block_next (objects->simulation);
    objects->model_reinit(objects->simulation->model, BLOCK, objects->simulation);
  }

  if (objects->model_sub_notepage != NULL) {
    gtk_widget_queue_draw(objects->model_sub_notepage);
  }

  GtkTreeSelection *sel;
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(objects->task_tree_view));
  gtk_tree_selection_select_path(sel, objects->simulation->current_trial_path);


}

static void model_controls_run_block_cb (GtkToolItem * tool_item,
					 ThreeTaskObjects * objects) {

  procedure_run_block (objects->simulation->model,
		       objects->simulation,
		       objects->model_run,
		       objects->model_reinit
		       );

  if (objects->model_sub_notepage != NULL) {
    gtk_widget_queue_draw(objects->model_sub_notepage);
  }

  GtkTreeSelection *sel;
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(objects->task_tree_view));
  gtk_tree_selection_select_path(sel, objects->simulation->current_trial_path);

}

static void model_controls_run_all_blocks_cb (GtkToolItem * tool_item,
					      ThreeTaskObjects * objects) {

  procedure_run_all_blocks (objects->simulation->model,
			    objects->simulation,
			    objects->model_run,
			    objects->model_reinit
			    );

  if (objects->model_sub_notepage != NULL) {
    gtk_widget_queue_draw(objects->model_sub_notepage);
  }

  GtkTreeSelection *sel;
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(objects->task_tree_view));
  gtk_tree_selection_select_path(sel, objects->simulation->current_trial_path);


}

static void setup_task_viewer_treeview (GtkTreeView * tree) {

  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;

  // aesthetic properties:
  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(tree), TRUE);

  // COlumns:
  // Block name
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Block", renderer,
						     "text", COL_BLOCK_NAME,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column);

  // Trial ID
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Trial ID", renderer,
						     "text", COL_TASK_ID,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column);


  // Input Patterns 
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Cue", renderer, "text",
						     COL_TASK_PATTERN_1,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Stim 1", renderer, "text",
						     COL_TASK_PATTERN_2,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Stim 2", renderer, "text",
						     COL_TASK_PATTERN_3,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Stim 3", renderer, "text",
						     COL_TASK_PATTERN_4,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);


  // Trial parameters
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Parameter 1", renderer, "text",
						     COL_TASK_PARAM_1,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Parameter 2", renderer, "text",
						     COL_TASK_PARAM_2,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);




}

static void model_controls_select_trial_treeview_cb (GtkTreeView * tree_view,
						     GtkTreePath * path,
						     GtkTreeViewColumn *column,
						     ThreeTaskObjects *objects) {
  GtkTreePath *path_copy;
  path_copy = gtk_tree_path_copy (path);

  procedure_change_trial (objects->simulation, objects->simulation->task_store, path_copy);
  objects->model_reinit(objects->simulation->model, TRIAL, objects->simulation);

  model_set_starting_activation (objects);


}


static GtkWidget* create_notepage_model_main(ThreeTaskObjects * objects) {

  GtkWidget *grid;
  GtkWidget *toolbar;
  GtkToolItem *tool_item;

  GtkWidget *grid_headerbar;
  GtkWidget *sub_notepage;
  GtkWidget *scrollwindow;
  GtkWidget *tree;

  // ------------- Task Browser ---------------
  scrollwindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_size_request (scrollwindow, TASK_VIEW_WIDTH, TASK_VIEW_HEIGHT);


  tree = gtk_tree_view_new();
  objects->task_tree_view = tree;
  gtk_tree_view_set_model (GTK_TREE_VIEW(tree), GTK_TREE_MODEL(objects->simulation->task_store));
  setup_task_viewer_treeview(GTK_TREE_VIEW(tree));
  // signal connect for selecting tree rows
  g_signal_connect (G_OBJECT(tree), "row-activated", 
		    G_CALLBACK(model_controls_select_trial_treeview_cb), (gpointer)objects);


  // handle selection for current trial

  GtkTreeSelection *sel;
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
  gtk_tree_selection_select_path(sel, objects->simulation->current_trial_path);


  // file import buttons here

  // tree view here
  gtk_container_add (GTK_CONTAINER(scrollwindow), tree);



  int position = 0; // toolbar position

  // ------------- TOOLBAR ----------------
  // Create a basic toolbar
  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

  // initialise
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_REFRESH);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
		    G_CALLBACK(model_controls_initialise_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Reinitialise model from parameters");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // step once
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
  		    G_CALLBACK(model_controls_step_once_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Process single model iteration");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // step many
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_FORWARD);
  //g_signal_connect (G_OBJECT(tool_item), "clicked", 
  //		    G_CALLBACK(model_controls_step_many_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Process multiple model iterations");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // run to end
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_NEXT);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
  		    G_CALLBACK(model_controls_run_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Run model trial to end");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // run to end, continue
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_OK);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
  		    G_CALLBACK(model_controls_continue_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), 
			      "Run to end, continue to next trial (carry over residual activation)");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // button here for run block
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_GO_FORWARD);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
  		    G_CALLBACK(model_controls_run_block_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), 
			      "Run block from current trial");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // button here for run all blocks
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_GOTO_LAST);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
  		    G_CALLBACK(model_controls_run_all_blocks_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), 
			      "Run all blocks from start");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);


  // options for toolbar
  gtk_widget_set_hexpand (toolbar, TRUE);
  gtk_widget_set_vexpand (toolbar, FALSE);

  // -------------- page header with current status -------

  grid_headerbar = gtk_grid_new();
  gtk_grid_set_column_spacing (GTK_GRID(grid_headerbar), 10);



  // spin button for controlling current trial  
  /* // NB commented out to remove dependency on simulation->subjects?
  current_trial_adjustment = gtk_adjustment_new (simulation->current_trial, 0, 
						 (simulation->subjects->
						 subj[simulation->current_subject]->num_fixed_trials)-1,
						 1, 0, 0);
  spin_button = gtk_spin_button_new (current_trial_adjustment, 1, 0);
  g_signal_connect (G_OBJECT(spin_button), "value-changed", G_CALLBACK(model_change_trial_cb), (gpointer) objects);
  objects->model_headerbar_spin_trial = spin_button;

  gtk_grid_attach (GTK_GRID(grid_headerbar), spin_button, 0, 0, 1, 1);
  */


  /*
  char textbuf[100];
  sprintf (textbuf, "Subject: %d", simulation->current_subject);
  label1 = gtk_label_new (textbuf);
  objects->model_headerbar_label_subject = label1; // keep track of label so we can update it
  gtk_grid_attach (GTK_GRID(grid_headerbar), label1, 1, 0, 1, 1);

  sprintf (textbuf, "Trial: %d", simulation->current_trial);
  label1 = gtk_label_new (textbuf);
  objects->model_headerbar_label_trial = label1; // keep track of label so we can update it
  gtk_grid_attach (GTK_GRID(grid_headerbar), label1, 2, 0, 1, 1);
  */


  /*
  // display input pattern, trialtype, ...?
  stroop_trial_data_print_as_string (textbuf, 100, 
				     fixed_block_trial_data_get(simulation->subjects,
								simulation->current_subject,
								simulation->current_trial));
  // printf ("%s", textbuf);
  label1 = gtk_label_new (textbuf);
  gtk_widget_set_size_request (label1, 300, 30); // max size so spin button is always in same place
  objects->model_headerbar_label_trial_data = label1; // keep track of label so we can update it
  gtk_grid_attach (GTK_GRID(grid_headerbar), label1, 3, 0, 1, 1);
  */

  

  // --------------- SUB-NOTEPAGE ------------------------    
  // Sub-notepage here: tabs for viewing network architecture and 
  // plotting single-trial activation
  sub_notepage = gtk_notebook_new();

  // keep a pointer to widgets so we can issue redraw signals on it
  objects->model_sub_notepage = sub_notepage;



  gtk_notebook_append_page(GTK_NOTEBOOK(sub_notepage),
			   create_sub_notepage_model_plot_activation(objects),
			   gtk_label_new("Plot Network Activation"));
  gtk_notebook_append_page(GTK_NOTEBOOK(sub_notepage),
			   create_sub_notepage_model_display_architecture(objects),
			   gtk_label_new("Display Network Architecture"));

			   
  

  // -------------- SHOW WIDGETS --------------------

  grid = gtk_grid_new();


  gtk_grid_attach (GTK_GRID(grid), toolbar, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID(grid), sub_notepage, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID(grid), scrollwindow, 1, 0, 1, 2);
  gtk_widget_set_vexpand (GTK_WIDGET(grid), TRUE);

  gtk_widget_show_all(grid);
  return (grid);
}



/*
static void main_quit (GtkWidget *window, ThreeTaskObjects *objects) {

  printf ("freeing objects\n");
  free (objects->simulation->model->activation_parameters); 
  pdp_model_free (objects->simulation->model);  // irritating segfault!

  printf ("freeing simulation\n");
  free_simulation (objects->simulation);

  printf ("freeing config files\n");
  g_free (objects->param_config_file);
  g_free (objects->task_config_file);

  printf ("freeing objects\n");
  g_free (objects);

  printf ("gtk main quit\n");
  gtk_main_quit ();

}
*/


int main (int argc, char *argv[]) {


  int c;
  char *model_confname = NULL, *task_confname = NULL;

  // process command line arguments (ie specify conf files)
    while ((c = getopt (argc, argv, "m:t:")) != -1) {
      switch (c) 
	{
	case 'm':
	  model_confname = optarg;
	  break;
	case 't':
	  task_confname = optarg;
	  break;
	case '?':
	  if (optopt == 'm' || optopt  == 't') {
	    printf ("option -%c requires name of a .conf file as an argument\n", optopt);
	  }
	  else if (isprint (optopt))
	    printf ("Unknown option -%c\n", optopt);
	  else
	    printf ("Unknown option character \\x%x \n", optopt);
	  return 1;
	}
    }


  gtk_init (&argc, &argv);

  ThreeTaskObjects * objects = g_malloc (sizeof(ThreeTaskObjects));
  // init function, set everything to null?



  objects->simulation = create_simulation();
  three_task_koch_conflict_parameters_htable_set_default (objects->simulation->model_params_htable);
  objects->simulation->datafile = fopen (DATAFILE, "a");
  objects->simulation->datafile_act = fopen (DATAFILE_ACT, "a");

  // define function pointer to model_run function 
  //  int (*model_run)(pdp_model*, ThreeTaskSimulation*);
  objects->model_run = &three_task_model_koch_conflict_run; // (model-specific, def in 3task_model_gs.c)

  //  int (*model_reinit)(pdp_model*, init_type, ThreeTaskSimulation*);
  objects->model_reinit = &three_task_model_koch_conflict_reinit; // (model-specific, def in 3task_model_gs.c)


  objects->model_sub_notepage = NULL;
  objects->task_tree_view = NULL;

  objects->param_config_file = create_param_import_objects();
  objects->task_config_file = triple_task_create_task_import_objects();

  // <--------------------- Temporary import of params and tasks to get us going ----------------->
  // Import parameters
  if (model_confname != NULL) 
    pdp_import_select_file (model_confname, objects->param_config_file);
  else 
    pdp_import_select_file ("3task_model_koch_conflict_params_default.conf", objects->param_config_file);

  pdp_load_from_file_short (objects->param_config_file);
  three_task_koch_conflict_parameters_import_commit (objects->param_config_file, objects->simulation->model_params_htable);

  // Import tasks
  if (task_confname != NULL) 
    pdp_import_select_file (task_confname, objects->task_config_file);
  else
    pdp_import_select_file ("3task_import_n2_basic.conf", objects->task_config_file);
  pdp_load_from_file_long (objects->task_config_file);
  triple_task_task_import_commit (objects->task_config_file, objects->simulation->task_store);
  // ----------------------------------------------------------------------------------------------


  // now create and build the model
  printf ("in main, building the model\n");
  objects->simulation->model = pdp_model_create (0, "3task_koch_conflict"); 
  init_model_koch_conflict_simulation (objects->simulation->model, objects->simulation->model_params_htable); 


  // Draw the GUI

  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *notes;

  // Create a window with a title, default size, and set border width
  if (!(window = gtk_window_new (GTK_WINDOW_TOPLEVEL))) {
    printf ("error, could not create toplevel window!\n");
    return 0;
  }
  gtk_window_set_title (GTK_WINDOW(window), "Three Task Switching: Model");
  gtk_window_set_default_size(GTK_WINDOW(window), MAIN_WINDOW_WIDTH_DEFAULT, MAIN_WINDOW_HEIGHT_DEFAULT);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width (GTK_CONTAINER(window), 10);
  // g_signal_connect (window, "destroy", G_CALLBACK(main_quit), (gpointer) objects); // causes irritating segfault
  g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // 


  // ------------- NOTEPAD -----------------

  notes = gtk_notebook_new();
 
  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_model_main(objects), 
			   gtk_label_new("Model"));

  /*
  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_import_model_params(objects), 
			   gtk_label_new("Parameter Import"));

  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_view_trials(objects), 
			   gtk_label_new("Task View"));

  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_import_trials(objects), 
			   gtk_label_new("Task Import"));
  */


  // Create a full-window grid to contain toolbar and the notebook
  grid = gtk_grid_new();
  
  // options for notebook
  gtk_widget_set_hexpand (notes, TRUE);
  gtk_widget_set_vexpand (notes, TRUE);
  gtk_grid_attach (GTK_GRID(grid), notes, 0, 1, 1, 1);

  gtk_container_add (GTK_CONTAINER(window), GTK_WIDGET(grid));

  gtk_widget_show_all (window);

  gtk_main();


  return 0;
}
