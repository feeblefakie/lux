#ifndef LUX_H
#define LUX_H

/**
 * @mainpage Lux: a simple and fast full-text search engine
 *
 * @section Introduction
 *
 * Lux is a search engine library.
 * Lux aims for a simple, fast and scalable search engine.
 *
 * The following classes are APIs for ordinary use.
 *
 * @li Lux::Engine
 * @li Lux::Indexer
 * @li Lux::Searcher
 * @li Lux::Document
 * @li Lux::Field
 * @li Lux::ResultSet
 * @li Lux::Condition
 * @li Lux::SortCondition
 * @li Lux::Paging
 * @li Lux::Config::Document
 * @li Lux::DocumentConfigParser
 *
 * [TODO] APIs for distributed index are not documented yet.
 *
 * project homepage: http://luxse.sourceforge.net/
 */

#include "luxconfig.h"
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "types.h"

#define LUX_META_DB "meta.db"
#define LUX_DOC_DB "doc_storage"
#define LUX_ID_DB "id_storage"

#endif
