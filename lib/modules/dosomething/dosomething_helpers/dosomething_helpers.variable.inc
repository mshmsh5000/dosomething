<?php

/**
 * @file
 * Code for Dosomething Helpers variable functionality.
 */

function dosomething_helpers_variable_form($form, &$form_state, $node) {
  $vars = dosomething_helpers_get_variables($node->nid);
  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );
  $form['styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Styles'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['styles']['alt_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Alt color'),
    '#description' => t('Hex value to use for headlines.'),
    '#default_value' => $vars['alt_color'],
    '#field_prefix' => '#',
    '#size' => 6,
  );
  $form['styles']['alt_bg_fid'] = array(
    //@todo: File Fid autocomplete?
    '#type' => 'textfield',
    '#title' => t('Alt BG File fid'),
    '#description' => t('File fid to display for BG pattern.'),
    '#default_value' => $vars['alt_bg_fid'],
    '#size' => 6,
  );
  $form['styles']['alt_image_campaign_cover_nid'] = array(
    //@todo: Node reference autocomplete.
    '#type' => 'textfield',
    '#title' => t('Alt header Image nid'),
    '#description' => t('An Image Node nid to display in the page header.'),
    '#default_value' => $vars['alt_image_campaign_cover_nid'],
    '#size' => 6,
  );
  $form['styles']['signup_form_submit_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Signup Form Button Label'),
    '#description' => t('Overrides the label on the Signup Form button.'),
    '#default_value' => $vars['signup_form_submit_label'],
    '#size' => 20,
  );
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => 'Save',
    ),
  );
  return $form;
}

function dosomething_helpers_variable_form_submit(&$form, &$form_state) {
  form_state_values_clean($form_state);
  $values = $form_state['values'];
  // Store hidden nid.
  $nid = $values['nid'];
  // Remove from values.
  unset($values['nid']);
  $node = node_load($nid);
  // Loop through all remaining values:
  foreach ($values as $name => $value) {
    // Save to the variable table.
    dosomething_helpers_set_variable($node, $name, $value);
  }
  drupal_set_message("Updated.");
}

/**
 * Returns array of all variable names.
 */
function dosomething_helpers_get_variable_names() {
  return array(
    'alt_bg_fid',
    'alt_color',
    'alt_image_campaign_cover_nid',
    'collect_num_participants',
    'mobilecommons_opt_in_path',
    'mobilecommons_friends_opt_in_path',
    'signup_form_submit_label',
    'sms_game_mp_story_id',
    'sms_game_mp_story_type',
  );
}

/**
 * Returns array of all dosomething_helper_variable records for a given $node.
 *
 * @return array
 *   Keyed by the variable name.
 */
function dosomething_helpers_get_variables($nid) {
  // Initialize array with NULL defaults.
  foreach (dosomething_helpers_get_variable_names() as $name) {
    $vars[$name] = NULL;
  }
  // Query for variable records for given $node.
  $result = db_select('dosomething_helpers_variable', 'v')
    ->fields('v')
    ->condition('entity_id', $nid)
    ->condition('entity_type', 'node')
    ->execute()
    ->fetchAll();
  foreach ($result as $record) {
    $vars[$record->name] = $record->value;
  }
  return $vars;
}

/**
 * Returns value of given $nid's $name variable.
 *
 * @return string
 *   All variable values are stored as strings.
 */
function dosomething_helpers_get_variable($nid, $name) {
  return db_select('dosomething_helpers_variable', 'v')
    ->fields('v', array('value'))
    ->condition('entity_id', $nid)
    ->condition('entity_type', 'node')
    ->condition('name', $name)
    ->execute()
    ->fetchField(0);
}

/**
 * Sets a given dosomething_helper variable $name to $value for given $node.
 */
function dosomething_helpers_set_variable($node, $name, $value) {
  // If a value is present:
  if (!empty($value)) {
    db_merge('dosomething_helpers_variable')
        ->key(array('entity_type' => 'node', 'entity_id' => $node->nid, 'name' => $name))
        ->fields(array(
            'entity_type' => 'node',
            'bundle' => $node->type,
            'entity_id' => $node->nid,
            'name' => $name,
            'value' => $value,
        ))
        ->execute();
  }
  // Else delete the value, to prevent storing an empty value.
  else {
    db_delete('dosomething_helpers_variable')
      ->condition('entity_id', $node->nid)
      ->condition('entity_type', 'node')
      ->condition('name', $name)
      ->execute();
  }
}

/**
 * Displays a node's relevant signup variables.
 */
function dosomething_helpers_third_party_variable_form($form, &$form_state, $node) {
  $nid = $node->nid;
  $form['optins'] = array(
    '#type' => 'fieldset',
    '#title' => t('Third Party Opt-ins'),
    '#description' => t('Custom campaign opt-in values.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // SMS Games have different variables.
  if (dosomething_campaign_get_campaign_type($node) == 'sms_game') {
    $value = dosomething_helpers_get_variable($nid, 'mobilecommons_opt_in_path');
    $form['optins']['mobilecommons'] = array(
      '#type' => 'textfield',
      '#title' => t('Mobilecommons Alpha Opt-in Path'),
      '#default_value' => $value,
      '#disabled' => TRUE,
    );
    $value = dosomething_helpers_get_variable($nid, 'mobilecommons_friends_opt_in_path');
    $form['optins']['mobilecommons_friends'] = array(
      '#type' => 'textfield',
      '#title' => t('Mobilecommons Beta Opt-in Path'),
      '#default_value' => $value,
      '#disabled' => TRUE,
    );
  }
  else {
    $value = dosomething_helpers_get_variable($nid, 'mailchimp_grouping_id');
    $form['optins']['mailchimp_grouping_id'] = array(
      '#type' => 'textfield',
      '#title' => t('MailChimp Grouping ID'),
      '#default_value' => $value,
      '#disabled' => TRUE,
    );
    $value = dosomething_helpers_get_variable($nid, 'mailchimp_group_name');
    $form['optins']['mailchimp_group_name'] = array(
      '#type' => 'textfield',
      '#title' => t('MailChimp Group Name'),
      '#default_value' => $value,
      '#disabled' => TRUE,
    );
    $value = dosomething_helpers_get_variable($nid, 'mobilecommons_opt_in_path');
    $form['optins']['mobilecommons'] = array(
      '#type' => 'textfield',
      '#title' => t('MobileCommons Opt-in Path'),
      '#default_value' => $value,
      '#disabled' => TRUE,
    );
  }

  // No submit button, this is a read only form.
  return $form;
}
