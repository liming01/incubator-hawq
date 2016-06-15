#include "postgres.h"

#include <sys/stat.h>
#include <unistd.h>

#include "access/htup.h"
#include "access/reloptions.h"
#include "access/sysattr.h"
#include "catalog/pg_foreign_table.h"
#include "commands/copy.h"
#include "commands/defrem.h"
#include "commands/explain.h"
#include "commands/vacuum.h"
#include "foreign/fdwapi.h"
#include "foreign/foreign.h"
#include "funcapi.h"
#include "miscadmin.h"
#include "nodes/makefuncs.h"
#include "optimizer/cost.h"
#include "optimizer/pathnode.h"
#include "optimizer/planmain.h"
#include "optimizer/restrictinfo.h"
#include "optimizer/var.h"
#include "utils/memutils.h"
#include "utils/rel.h"

PG_MODULE_MAGIC;

typedef struct HelloFdwExecutionState {
  int rownum;
} HelloFdwExecutionState;

extern Datum hello_world_fdw_handler(PG_FUNCTION_ARGS);
extern Datum hello_world_fdw_validator(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(hello_world_fdw_handler);
PG_FUNCTION_INFO_V1(hello_world_fdw_validator);

static void hwGetForeignRelSize(PlannerInfo*, RelOptInfo*, Oid);
static void hwGetForeignPaths(PlannerInfo*, RelOptInfo *baserel, Oid foreigntableid);
static ForeignScan *hwGetForeignPlan(PlannerInfo*, RelOptInfo*, Oid, ForeignPath*, List*, List*);
static void hwExplainForeignScan(ForeignScanState*, struct ExplainState*);
static void hwBeginForeignScan(ForeignScanState*, int);
static TupleTableSlot *hwIterateForeignScan(ForeignScanState*);
static void hwReScanForeignScan(ForeignScanState*);
static void hwEndForeignScan(ForeignScanState*);

Datum hello_world_fdw_handler(PG_FUNCTION_ARGS) {
  FdwRoutine *fdwroutine = makeNode(FdwRoutine);
  fdwroutine->GetForeignRelSize = hwGetForeignRelSize;
  fdwroutine->GetForeignPaths = hwGetForeignPaths;
  fdwroutine->GetForeignPlan = hwGetForeignPlan;
  fdwroutine->ExplainForeignScan = hwExplainForeignScan;
  fdwroutine->BeginForeignScan = hwBeginForeignScan;
  fdwroutine->IterateForeignScan = hwIterateForeignScan;
  fdwroutine->ReScanForeignScan = hwReScanForeignScan;
  fdwroutine->EndForeignScan = hwEndForeignScan;
  PG_RETURN_POINTER(fdwroutine);
}

Datum hello_world_fdw_validator(PG_FUNCTION_ARGS) {
  PG_RETURN_VOID();
}

static void
hwGetForeignRelSize(PlannerInfo *root,
                    RelOptInfo *baserel,
                    Oid foreigntableid) {
  baserel->rows = 1;
  baserel->width = 11;
}

static void
hwGetForeignPaths(PlannerInfo *root,
                  RelOptInfo *baserel,
                  Oid foreigntableid) {}

static ForeignScan *
hwGetForeignPlan(PlannerInfo *root,
                 RelOptInfo *baserel,
                 Oid foreigntableid,
                 ForeignPath *best_path,
                 List *tlist,
                 List *scan_clauses) {
  scan_clauses = extract_actual_clauses(scan_clauses, false);
  return make_foreignscan(tlist,
                          scan_clauses,
                          baserel->relid,
                          NIL,
                          best_path->fdw_private,
                          NIL,
                          NIL,
                          NIL);
}

static void
hwExplainForeignScan(ForeignScanState *node,
                     struct ExplainState *es) {
  //ExplainPropertyText("Hello", "World", es);
}

static void
hwBeginForeignScan(ForeignScanState *node,
                   int eflags) {
  if(eflags & EXEC_FLAG_EXPLAIN_ONLY) return;
}

static TupleTableSlot *
hwIterateForeignScan(ForeignScanState *node) {
  TupleTableSlot *slot = node->ss.ss_ScanTupleSlot;
  Relation rel;
  AttInMetadata  *attinmeta;
  HeapTuple tuple;
  HelloFdwExecutionState *hestate = (HelloFdwExecutionState *) node->fdw_state;
  int i;
  int natts;
  char **values;

  if( hestate->rownum != 0 ){
    ExecClearTuple(slot);
    return slot;
  }

  rel = node->ss.ss_currentRelation;

  //
  ForeignTable *table = GetForeignTable(RelationGetRelid(rel));
  List     *options;
  ListCell   *lc;
  options = list_concat(options, table->options);
  printf("uuuuuuuuuuuuuuuuuuuuuuuu\n");
  foreach(lc, options) {
    DefElem    *def = (DefElem *) lfirst(lc);
    printf("cccc %s\n", def->defname);
  }
  //

  attinmeta = TupleDescGetAttInMetadata(rel->rd_att);

  natts = rel->rd_att->natts;
  printf("%d\n", natts);
  values = (char **) palloc(sizeof(char *) * natts);

  for(i = 0; i < natts; i++ ){
    values[i] = "Hello, World";
  }

  tuple = BuildTupleFromCStrings(attinmeta, values);
  ExecStoreGenericTuple(tuple, slot, true);

  hestate->rownum++;

  return slot;
}

static void
hwReScanForeignScan(ForeignScanState *node) {}

static void
hwEndForeignScan(ForeignScanState *node) {}
