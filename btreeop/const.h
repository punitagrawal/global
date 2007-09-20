/* This file is generated automatically by convert.pl from btreeop/manual.in. */
const char *progname = "btreeop";
const char *usage_const = "Usage: btreeop [-A][-C][-D[n] key][-K[n] key][-L[2]][-k prefix][dbname]\n";
const char *help_const = "Commands:\n\
none\n\
       Read records sequentially.\n\
-A\n\
       Append records. If database doesn't exist, btreeop creates it.\n\
-C\n\
       Create database and write records to it.\n\
-D[n] key\n\
       Delete records by the key. By default, n is 0 (primary key).\n\
-K[n] key\n\
       Search records by the key. By default, n is 0 (primary key).\n\
-L[2]\n\
       List all primary keys.\n\
       If 2 is specified, list all the key and data pairs.\n\
dbname\n\
       Database name. By default, it assume btree.\n\
Options:\n\
-k prefix\n\
       Scan records which have the prefix as a primary key.\n\
       This option is valid only with sequential read operation\n\
       -L command or no command.\n\
";
