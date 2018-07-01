UPDATE system SET filename = REPLACE(filename, 'sites/all/modules', 'sites/all/modules/contrib');
UPDATE registry SET filename = REPLACE(filename, 'sites/all/modules', 'sites/all/modules/contrib');
UPDATE registry_file SET filename = REPLACE(filename, 'sites/all/modules', 'sites/all/modules/contrib');

UPDATE system SET filename = REPLACE(filename, 'sites/all/modules/contrib', 'sites/all/modules/features') WHERE name LIKE 'myisrael_backbone%';
UPDATE registry SET filename = REPLACE(filename, 'sites/all/modules/contrib', 'sites/all/modules/features') WHERE name LIKE 'myisrael_backbone%';
UPDATE registry_file SET filename = REPLACE(filename, 'sites/all/modules/contrib', 'sites/all/modules/features') WHERE filename LIKE '%myisrael_backbone%';

UPDATE system SET filename = REPLACE(filename, 'sites/all/modules/dev', 'sites/all/modules/dev') WHERE name LIKE 'devel%';
UPDATE registry SET filename = REPLACE(filename, 'sites/all/modules/dev', 'sites/all/modules/dev') WHERE name LIKE 'devel%';
UPDATE registry_file SET filename = REPLACE(filename, 'sites/all/modules/dev', 'sites/all/modules/dev') WHERE filename LIKE '%devel%';

TRUNCATE `cache`;
TRUNCATE `cache_admin_menu`;
TRUNCATE `cache_block`;
TRUNCATE `cache_bootstrap`;
TRUNCATE `cache_features`;
TRUNCATE `cache_field`;
TRUNCATE `cache_filter`;
TRUNCATE `cache_form`;
TRUNCATE `cache_image`;
TRUNCATE `cache_libraries`;
TRUNCATE `cache_menu`;
TRUNCATE `cache_page`;
TRUNCATE `cache_path`;
TRUNCATE `cache_views`;
TRUNCATE `cache_views_data`;