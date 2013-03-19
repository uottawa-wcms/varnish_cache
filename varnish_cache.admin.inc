<?php

function varnish_cache_admin_settings($form, &$form_state) {
  module_load_include('inc', 'varnish_cache');
  $form['varnish_servers'] = array(
    '#type' => 'textarea',
    '#title' => t('Varnish Servers'),
    '#description' => t('Enter one server IP address or host name per line'),
    '#default_value' => implode("\n", _varnish_cache_get_cache_servers()),
  );
  $form['debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug Messages'),
    '#description' => t('Checking this will add additional debugging messages'),
    '#default_value' => variable_get('varnish_cache_debug', FALSE),
  );
  $form['secure_varnish'] = array(
    '#type' => 'checkbox',
    '#title' => t('HTTPS'),
    '#description' => t('Checking this will cause HTTPS to be used for PURGE and BAN requests'),
    '#default_value' => variable_get('varnish_cache_use_https', FALSE),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );
}

function varnish_cache_admin_settings_submit($form, &$form_state) {
  $servers_raw = explode("\n", $form_state['values']['varnish_servers']);
  $servers = array();
  foreach($servers_raw as $srv) {
    $servers[] = trim($srv);
  }
  variable_set('varnish_cache_servers', $servers);
  variable_set('varnish_cache_debug', !!$form_state['values']['debug_mode']);
  variable_set('varnish_cache_use_https', !!$form_state['values']['secure_varnish']);
  drupal_set_message(t('Varnish caching servers have been saved'));
}