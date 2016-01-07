<?php


/**
 * Implements hook_form().
 */
function thumbor_admin_form($form, &$form_state) {

  $form['thumbor_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbor Server'),
    '#default_value' => variable_get('thumbor_server'),
    '#size' => 60,
    '#description' => t('The URL of thumbor server.'),
    '#required' => TRUE,
  );

  $form['thumbor_key'] = array(
    '#type' => 'textarea',
    '#title' => t('Thumbor Key'),
    '#default_value' => variable_get('thumbor_key'),
    '#description' => t('The Key of thumbor server.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}