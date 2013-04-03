<?php

function _varnish_cache_get_cache_servers() {
  return variable_get('varnish_cache_servers', array());
}

function _varnish_cache_purge_all($host_ip) {
  global $base_url;
  $urldata = parse_url($base_url);
  $c = curl_init();
  $headers = array('Host: ' . $urldata['host']);
  curl_setopt($c, CURLOPT_HTTPHEADER, $headers);
  curl_setopt($c, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($c, CURLOPT_URL, $base_url . '/');
  curl_setopt($c, CURLOPT_HEADER, FALSE);
  curl_setopt($c, CURLOPT_CUSTOMREQUEST, 'BAN');
  curl_exec($c);
}

function _varnish_cache_purge($url, $host_ip) {
drupal_set_message('preparing: ' . $url);
  if (!strpos($url, '://')) {
    global $base_url;
    if (substr($url, 0, 1) != '/') {
      $url = '/' . $url;
    }
    $url = $base_url . $url;
  }
  $c = curl_init();
  $urldata = parse_url($url);
  $url = 'http://' . $host_ip . $urldata['path'];
  if (!empty($urldata['query'])) {
    $url .= '?' . $urldata['query'];
  }
  $headers = array('Host: ' . $urldata['host']);
  curl_setopt($c, CURLOPT_HTTPHEADER, $headers);
  curl_setopt($c, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($c, CURLOPT_URL, $url);
  curl_setopt($c, CURLOPT_HEADER, FALSE);
  curl_setopt($c, CURLOPT_CUSTOMREQUEST, 'PURGE');
  curl_exec($c);
  drupal_set_message('Varnish error dump: ' . curl_error($c));
  drupal_set_message('Varnish cache purge on ' . $url . ' to ' . $host_ip . ', response: ' . curl_getinfo($c, CURLINFO_HTTP_CODE));
}
