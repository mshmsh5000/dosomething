<?php
/**
 * @file
 * dosomething_taxonomy.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dosomething_taxonomy_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_partner_info|field_collection_item|field_partners|form';
  $field_group->group_name = 'group_partner_info';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_partners';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Info',
    'weight' => '3',
    'children' => array(
      0 => 'field_image_partner',
      1 => 'field_partner_copy',
      2 => 'field_video',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Info',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-partner-info field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_partner_info|field_collection_item|field_partners|form'] = $field_group;

  return $export;
}
