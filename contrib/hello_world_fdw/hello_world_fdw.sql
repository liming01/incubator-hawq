CREATE FUNCTION hello_world_fdw_handler()
RETURNS fdw_handler
AS 'HAWQ_HOME/contrib/hello_world_fdw/hello_world_fdw.so'
LANGUAGE C STRICT;

CREATE FUNCTION hello_world_fdw_validator(text[], oid)
RETURNS void
AS '$HAWQ_HOME/contrib/hello_world_fdw/hello_world_fdw.so'
LANGUAGE C STRICT;

CREATE FOREIGN DATA WRAPPER hello_world_fdw
HANDLER 'hello_world_fdw_handler'
VALIDATOR 'hello_world_fdw_validator';

-- clean up
DROP FUNCTION hello_world_fdw_handler();
DROP FUNCTION hello_world_fdw_validator();
DROP FOREIGN DATA WRAPPER hello_world_fdw;
