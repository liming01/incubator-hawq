/*-------------------------------------------------------------------------
 *
 * pg_foreign_data_wrapper.h
 *	  definition of the system "foreign-data wrapper" relation (pg_foreign_data_wrapper)
 *	  along with the relation's initial contents.
 *
 *
 * Copyright (c) 2009-2010, Greenplum Inc
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/catalog/pg_foreign_data_wrapper.h,v 1.3 2009/02/24 10:06:34 petere Exp $
 *
 * NOTES
 *	  the genbki.sh script reads this file and generates .bki
 *	  information from the DATA() statements.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_FOREIGN_DATA_WRAPPER_H
#define PG_FOREIGN_DATA_WRAPPER_H

#include "catalog/genbki.h"

/* TIDYCAT_BEGINFAKEDEF

   CREATE TABLE pg_foreign_data_wrapper
   with (camelcase=ForeignDataWrapper, relid=2898, reltype_oid=6447)
   (
   fdwname       name, -- foreign-data wrapper name
   fdwowner      oid, -- FDW owner
   fdwhandler	 oid, -- FDW handler function
   fdwvalidator  oid, -- optional validation function

   -- VARIABLE LENGTH FIELDS start here.

   fdwacl        aclitem[], -- access permissions
   fdwoptions    text[] -- FDW options
   );

   create unique index on pg_foreign_data_wrapper(oid) with (indexid=3306, CamelCase=ForeignDataWrapperOid, syscacheid=FOREIGNDATAWRAPPEROID, syscache_nbuckets=8);

   create unique index on pg_foreign_data_wrapper(fdwname) with (indexid=3307, CamelCase=ForeignDataWrapperName, syscacheid=FOREIGNDATAWRAPPERNAME, syscache_nbuckets=8);

   alter table pg_foreign_data_wrapper add fk fdwowner on pg_authid(oid);
   alter table pg_foreign_data_wrapper add fk fdwhandler on pg_proc(oid);
   alter table pg_foreign_data_wrapper add fk fdwvalidator on pg_proc(oid);

   TIDYCAT_ENDFAKEDEF
*/

/* ----------------
 *		pg_foreign_data_wrapper definition.  cpp turns this into
 *		typedef struct FormData_pg_foreign_data_wrapper
 * ----------------
 */
#define ForeignDataWrapperRelationId	2898

CATALOG(pg_foreign_data_wrapper,2898)
{
	NameData	fdwname;		/* foreign-data wrapper name */
	Oid			fdwowner;		/* FDW owner */
	Oid			fdwhandler;		/* handler function, or 0 if none */
	Oid			fdwvalidator;	/* option validation function, or 0 if none */

	/* VARIABLE LENGTH FIELDS start here. */

	aclitem		fdwacl[1];		/* access permissions */
	text		fdwoptions[1];	/* FDW options */
} FormData_pg_foreign_data_wrapper;

/* ----------------
 *		Form_pg_fdw corresponds to a pointer to a tuple with
 *		the format of pg_fdw relation.
 * ----------------
 */
typedef FormData_pg_foreign_data_wrapper *Form_pg_foreign_data_wrapper;

/* ----------------
 *		compiler constants for pg_fdw
 * ----------------
 */

#define Natts_pg_foreign_data_wrapper				6
#define Anum_pg_foreign_data_wrapper_fdwname		1
#define Anum_pg_foreign_data_wrapper_fdwowner		2
#define Anum_pg_foreign_data_wrapper_fdwhandler		3
#define Anum_pg_foreign_data_wrapper_fdwvalidator	4
#define Anum_pg_foreign_data_wrapper_fdwacl			5
#define Anum_pg_foreign_data_wrapper_fdwoptions		6

#endif   /* PG_FOREIGN_DATA_WRAPPER_H */
