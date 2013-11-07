void test_layer_code () {


  /* Create a layer and set the activation values of some nodes */
    a_layer_tail = layer_create(3);
    a_layer_head = a_layer_tail;
    a_layer_tail->units[0] = 0.5;
    a_layer_tail->units[1] = -0.5;
    a_layer_tail->units[2] = 1;



     printf ("\n");
     printf ("red = %2.1f, green = %2.1f, blue = %2.1f\n",
            a_layer_tail->units[0], a_layer_tail->units[1], a_layer_tail->units[2]);

    /* now iterate the layer multiplying by 1.2 */


    double input_activation[3] = {100, 100, 0};

    int i;
    for (i = 0; i < 10; i++) {
        pdp_layer * tmp = a_layer_tail;
        a_layer_tail = layer_create(3);
        a_layer_tail->previous = tmp;
        tmp->next = a_layer_tail;

        int u;
        for (u = 0; u < a_layer_tail->size; u++) {
            a_layer_tail->units[u] = act_gs(input_activation[u], a_layer_tail->previous->units[u],
                                            STEP_SIZE, ACT_MAX, ACT_MIN);
        }
    }


    printf ("dump data backwards:\n");

    pdp_layer * layer_iterator = a_layer_tail;
    while (layer_iterator != NULL) {
        printf ("red = %2.1f, green = %2.1f, blue = %2.1f\n", 
		layer_iterator->units[0], layer_iterator->units[1], layer_iterator->units[2]);
        layer_iterator = layer_iterator->previous;
    }

    printf ("dump data forwards:\n");

    layer_iterator = a_layer_head;
    while (layer_iterator != NULL) {
        printf ("red = %2.1f, green = %2.1f, blue = %2.1f\n",
                layer_iterator->units[0], layer_iterator->units[1], layer_iterator->units[2]);
        layer_iterator = layer_iterator->next;
    }

    layer_free_backward (a_layer_tail);
    
    return;
}
