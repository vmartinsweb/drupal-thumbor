<?php


/**
 * Implements hook_form().
 */
function thumbor_form_callback($form_state) {
    $form = array();

    $form['trim'] = array(
        '#type' => 'fieldset',
        '#title' => t('Trim'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#description' => t('Removing surrounding space in images can be done using the trim option.'),
    );

    $form['trim']['orientation'] = array(
        '#type' => 'select',
        '#title' => t('Orientation'),
        '#description' => t('Where to get the pixel color.'),
        '#options' => array('none'=> 'None', 'trim' => 'Trim', 'trim:top-left'=> 'Trim: top-left', 'trim:bottom-right'=>'Trim: bottom-right'),
        '#default_value' => $form_state['trim']['orientation'],
    );

    $form['trim']['tolerance'] = array(
        '#type' => 'textfield',
        '#title' => t('Tolerance'),
        '#size' => 10,
        '#maxlength' => 10,
        '#required' => FALSE,
        '#description' => t('For a RGB image the tolerance would be within the range 0-442.'),
        '#default_value' => $form_state['trim']['orientation'] != 'none' ? $form_state['trim']['tolerance'] : NULL,
        '#element_validate' => array('thumbor_tolerance_validate'),
    );

    $form['smart'] = array(
        '#type' => 'checkbox',
        '#title' => t('Smart'),
        '#required' => FALSE,
        '#description' => t('Smart means using smart detection of focal points.'),
        '#default_value' => $form_state['smart'],
    );

    $form['fit_in'] = array(
        '#type' => 'select',
        '#title' => t('Fit-in'),
        '#description' => t('Fit-in means that the generated image should not be auto-cropped and otherwise just fit in an imaginary box specified by Width x Height. If a full fit-in is specified, then the largest size is used for cropping (width instead of height, or the other way around)'),
        '#options' => array('none'=> 'None', 'fit-in' => 'Fit-in', 'full-fit-in'=> 'Full fit-in', 'adaptive-fit-in'=>'Adaptive fit-in', 'adaptive-full-fit-in'=>'Adaptive full fit-in'),
        '#default_value' => $form_state['fit_in'],
        '#element_validate' => array('thumbor_fit_in_validate'),
    );
    
    $form['image_size'] = array(
        '#type' => 'fieldset',
        '#title' => t('Image size'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#description' => t('Manually crop the image at left-top and right-bottom.'),
    );

    $form['image_size']['width'] = array(
        '#type' => 'textfield',
        '#title' => t('Width'),
        '#size' => 10,
        '#maxlength' => 10,
        '#required' => TRUE,
        '#description' => t('The default value (in case it is omitted) for this option is to use proportional size (0) to the original image'),
        '#default_value' => !empty($form_state['image_size']['width']) ? $form_state['image_size']['width'] : 0,
    );

    $form['image_size']['lenght'] = array(
        '#type' => 'textfield',
        '#title' => t('Lenght'),
        '#size' => 10,
        '#maxlength' => 10,
        '#required' => TRUE,
        '#description' => t('The default value (in case it is omitted) for this option is to use proportional size (0) to the original image'),
        '#default_value' => !empty($form_state['image_size']['lenght']) ? $form_state['image_size']['lenght'] : 0,
    );

    $form['manual_crop'] = array(
        '#type' => 'fieldset',
        '#title' => t('Manual crop'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#description' => t('Manually crop the image at left-top and right-bottom.'),
    );

    $form['manual_crop']['left'] = array(
        '#type' => 'textfield',
        '#title' => t('Left'),
        '#size' => 10,
        '#maxlength' => 10,
        '#required' => TRUE,
        '#default_value' => !empty($form_state['manual_crop']['left']) ? $form_state['manual_crop']['left'] : 0,
    );

    $form['manual_crop']['top'] = array(
        '#type' => 'textfield',
        '#title' => t('Top'),
        '#size' => 10,
        '#maxlength' => 10,
        '#required' => TRUE,
        '#default_value' => !empty($form_state['manual_crop']['top']) ? $form_state['manual_crop']['top'] : 0,
    );

    $form['manual_crop']['right'] = array(
        '#type' => 'textfield',
        '#title' => t('Right'),
        '#size' => 10,
        '#maxlength' => 10,
        '#required' => TRUE,
        '#default_value' => !empty($form_state['manual_crop']['right']) ? $form_state['manual_crop']['right'] : 0,
    );

    $form['manual_crop']['bottom'] = array(
        '#type' => 'textfield',
        '#title' => t('Left'),
        '#size' => 10,
        '#maxlength' => 10,
        '#required' => TRUE,
        '#default_value' => !empty($form_state['manual_crop']['bottom']) ? $form_state['manual_crop']['bottom'] : 0,
    );

    $form['crop_align'] = array(
        '#type' => 'fieldset',
        '#title' => t('Crop align'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#description' => t('Alignment of crop.'),
    );

    $form['crop_align']['horizontal'] = array(
        '#type' => 'select',
        '#title' => t('Horizontal'),
        '#description' => t('The default value (in case it is omitted) for this option is "center".'),
        '#options' => array('default'=> 'Default', 'left' => 'Left', 'center'=> 'Center', 'right' => 'Right'),
        '#default_value' => $form_state['crop_align']['horizontal'],
    );

    $form['crop_align']['vertical'] = array(
        '#type' => 'select',
        '#title' => t('Vertical'),
        '#description' => t('The default value (in case it is omitted) for this option is "middle".'),
        '#options' => array('default'=> 'Default', 'top' => 'Top', 'middle'=> 'middle', 'bottom' => 'Bottom'),
        '#default_value' => $form_state['crop_align']['vertical'],
    );

    $form['filters'] = array(
        '#type' => 'textfield',
        '#title' => t('Filters'),
        '#size' => 120,
        '#maxlength' => 255,
        '#required' => FALSE,
        '#description' => t('Filters can be applied sequentially to the image before returning. Ex. FILTERNAME(ARGUMENT):FILTERNAME(ARGUMENT)'),
        '#default_value' => $form_state['filters'],
    );

    return $form;
}

function thumbor_fit_in_validate($element, $form_state) {
    if ($form_state['values']['data']['fit_in'] != 'none' and $form_state['values']['data']['image_size']['width'] == 0 and $form_state['values']['data']['image_size']['height'] == 0) {
        form_error($element, t('When using fit-in or full-fit-in, you must specify width and/or height image size.'));
    }
}

function thumbor_tolerance_validate($element, $form_state) {
    if (empty($form_state['values']['data']['trim']['tolerance'])) {
        return;
    }
    if ($form_state['values']['data']['trim']['tolerance'] < 0 or $form_state['values']['data']['trim']['tolerance'] > 422) {
        form_error($element, t('For a RGB image the tolerance would be within the range 0-442.'));
    }
}