<?php
// $Id$

/**
 * @file
 * content type to display nodes of remote website.
 */

/**
 * Callback function to supply a list of content types.
 */
function ctools_remote_node_remote_node_ctools_content_types() {
  return array(
    'single' => TRUE,
    'no title override' => TRUE,
    'title' => t('Remote node'),
    'icon' => 'icon_remote_node.png',
    'description' => t('Add a node from remote site as content.'),
    // Make this a top level category so it appears higher in UIs that support
    // that.
    'top level' => TRUE,
    'category' => t('Custom'),
    'defaults' => array('remote_node_id' => -1),
		// add form callback
		'add form' => array(
			'remote_node_content_type_form' => t('Content type'),
			'remote_node_content_edit_form' => t('Content')
		),
		// edit form callback
		'edit form' => 'remote_node_content_edit_form',
    // render callback is automatically deduced:
    // 'render callback' => 'ctools_custom_content_type_render',
    'all contexts' => TRUE,
  );
}

/**
 * Output function for the 'remote_node' content type. Outputs a custom
 * based on the module and delta supplied in the configuration.
 */
function ctools_remote_node_remote_node_content_type_render($subtype, $conf, $args, $contexts) {
  static $delta = 0;

  $block = ctools_remote_node_remote_node_make_block($conf);
  $block->subtype = ++$delta;

  return $block;
}

/**
 * Callback to provide the administrative title of the static content.
 */
function ctools_remote_node_remote_node_content_type_admin_title($subtype, $conf) {
  $output = t('Custom');

  // load remote node
  $node = node_remote_load($conf['remote_node_id']);
  if (!empty($node['title'])) {
    $output .= " (" . filter_xss_admin($node['title']) . ")";
  }

  return $output;
}

/**
 * Callback to provide administrative info. In this case we'll render the
 * content as long as it's not PHP, which is too risky to render here.
 */
function ctools_remote_node_remote_node_content_type_admin_info($subtype, $conf) {
  $block = ctools_remote_node_remote_node_make_block($conf);

  return $block;
}

/**
 * The form to choose content type.
 */
function remote_node_content_type_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  // Select the type of remote node
  $form['remote_node_type'] = array(
    '#title' => t('Node type'),
    '#type' => 'radios',
    '#options' => node_remote_types('names'),
    '#description' => t('Select the type of remote node.'),
  );

  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function remote_node_content_type_form_submit(&$form, &$form_state) {
	$form_state['conf']['remote_node_type'] = $form_state['values']['remote_node_type'];
}

/**
 * The form to select node.
 */
function remote_node_content_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

	$node_type = isset($form_state['values']['remote_node_type'])?$form_state['values']['remote_node_type']:$conf['remote_node_type'];

  // Select remote content to display
  $form['remote_node_id'] = array(
    '#title' => t('Remote Node'),
    '#type' => 'select',
    '#default_value' => $conf['remote_node_id'],
    '#options' => node_remote_nodes($node_type),
    '#description' => t('Select the page.'),
  );

  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function remote_node_content_edit_form_submit(&$form, &$form_state) {
	$form_state['conf']['remote_node_id'] = $form_state['values']['remote_node_id'];
}

/**
 * Remote Node Helper functions.
 */

/**
 * Construct the block to display(in panels, etc.)
 */
function ctools_remote_node_remote_node_make_block($conf) {
  // load remote node
  $node = node_remote_load($conf['remote_node_id']);

  $block          = new stdClass();
  $block->title   = $node['title'];
  $block->content = $node['body'];

  return $block;
}

