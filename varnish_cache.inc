<?php

function _varnish_cache_get_cache_servers() {
  return variable_get('varnish_cache_servers', array());
}

function _varnish_cache_purge_all($host_ip) {
  global $base_url;
  $urldata = parse_url($base_url);
  $c = curl_init();
  if (variable_get('varnish_cache_use_https')) {
    $url = 'https://' . $host_ip . '/';
  } else {
    $url = 'http://' . $host_ip . '/';
  }
  _varnish_cache_make_request($urldata['host'], $url, 'BAN');
}

function _varnish_cache_purge($url, $host_ip) {
  if (!strpos($url, '://')) {
    global $base_url;
    if (substr($url, 0, 1) != '/') {
      $url = '/' . $url;
    }
    $url = $base_url . $url;
  }
  $c = curl_init();
  $urldata = parse_url($url);
  if (variable_get('varnish_cache_use_https')) {
    $url = 'https://' . $host_ip . $urldata['path'];
  } else {
    $url = 'http://' . $host_ip . $urldata['path'];
  }
  if (!empty($urldata['query'])) {
    $url .= '?' . $urldata['query'];
  }
  _varnish_cache_make_request($urldata['host'], $url, 'PURGE');
}

function _varnish_cache_make_request($host, $url, $method = 'PURGE') {
  varnish_cache_debug('Making curl request to @url [host header: @host] via @method', array(
    '@url' => $url,
    '@host' => $host,
    '@method' => $method,
  ));
  $c = curl_init();
  $headers = array('Host: ' . $host);
  curl_setopt($c, CURLOPT_HTTPHEADER, $headers);
  curl_setopt($c, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($c, CURLOPT_URL, $url);
  curl_setopt($c, CURLOPT_HEADER, FALSE);
  curl_setopt($c, CURLOPT_CUSTOMREQUEST, $method);
  curl_exec($c);
  varnish_cache_debug('Result of curl @method request to @url [@host]: @status', array(
    '@url' => $url,
    '@method' => $method,
    '@host' => $host,
    '@status' => curl_getinfo($c, CURLINFO_HTTP_CODE),
  ));
}
