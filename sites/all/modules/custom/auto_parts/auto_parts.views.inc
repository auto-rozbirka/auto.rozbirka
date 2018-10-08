<?php
/**
 * @file
 * Views include file with views hooks.
 */
p(1,1);
/**
 * Implementation of hook_views_plugins().
 */
function auto_parts_views_plugins() {

  $path = drupal_get_path('module', 'auto_parts');

  $style_defaults = array(
    'path' => $path . '/plugins',
    'parent' => 'views_data_export',
    'theme' => 'views_data_export',
    'theme path' => $path . '/theme',
    'theme file' => 'views_data_export.theme.inc',
    'uses row plugin' => FALSE,
    'uses fields' => TRUE,
    'uses options' => TRUE,
    'type' => 'data_export',
  );

  return array(
    'display' => array (
      '' => array(
        'title' => t('Auto export to excel'),
        'help' => t('Auto export to excel'),
        'path' => $path . '/plugins',
        'handler' => 'auto_parts_plugin_display_export',
        'parent' => 'feed',
        'uses hook menu' => TRUE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('Auto export to excel'),
        'help topic' => 'auto_parts-data-export',
      ),
    ),
    'style' => array(
      'views_data_export' => array(
        // this isn't really a display but is necessary so the file can
        // be included.
        'no ui' => TRUE,
        'handler' => 'views_data_export_plugin_style_export',
        'path' => $path . '/plugins',
        'theme path' => $path . '/theme',
        'theme file' => 'views_data_export.theme.inc',
        'type' => 'normal',
      ),
      'auto_parts_excel' => array(
        'title' => t('excel file'),
        'help' => t('Display the view as a comma separated list.'),
        'handler' => 'excel_plugin_style_export_excel',
        // Views Data Export element that will be used to set additional headers when serving the feed.
        'export headers' => array('Content-type' => 'text/csv; charset=utf-8'),
        // Views Data Export element mostly used for creating some additional classes and template names.
        'export feed type' => 'excel',
        'export feed text' => 'excel',
        'export feed file' => '%view.excel',
        'export feed icon' => drupal_get_path('module', 'views_data_export') . '/images/csv.png',
        'additional themes' => array(
          'auto_parts_excel_header' => 'style',
          'auto_parts_excel_body' => 'style',
          'auto_parts_excel_footer' => 'style',
        ),
        'additional themes base' => 'views_data_export_csv',
      ) + $style_defaults,
    ),
  );
}
