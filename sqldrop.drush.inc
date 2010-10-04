<?php

/**
 * @file
 * Drush command for dropping all tables in Drupal's database. This is done by
 * piping the output of mysqldump, filtering the DROP TABLE lines and sending
 * the result back into MySQL.
 *
 * http://drupalcontrib.org/api/drupal/contributions--drush--examples--sandwich.drush.inc/6/source
 */


/**
 * Implementation of hook_drush_command().
 */
function sqldrop_drush_command() {
  $items = array();

  $items['sql-drop'] = array(
    'description' => "Drops all tables in Drupal's database.",
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function sqldrop_drush_help($section) {
  switch ($section) {
    case 'drush:sql-drop':
      return dt("Running sql-drop will make Drush DROP all the database's tables.");
  }
}

/**
 * Drush command callback.
 *
 */
function drush_sql_drop() {
  $query = _drush_sql_drop_query_tables();

  $tables = db_query($query);
  while ($table = db_fetch_object($tables)) {
    $table_name = reset($table);

    db_query('DROP TABLE '. $table_name);
  }
}

/**
 * Helper function for fetching the proper SHOW TABLES query.
 */
function _drush_sql_drop_query_tables() {
  $scheme = _drush_sql_get_scheme($db_spec);

  if ($scheme == 'pgsql') {
    return "select tablename from pg_tables where schemaname='public'";
  }

  return 'SHOW TABLES';
}
