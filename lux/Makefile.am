INCLUDES = -I. -I./Document -I./Lexer -I./Query -I./StorageEngine \
           -I./LangUtil -I./Storage -I./StorageInterface

lib_LTLIBRARIES = liblux.la

liblux_la_SOURCES = Lexer/MecabMA.cpp Storage/InvertedIndex.cpp \
                    StorageInterface/Indexer.cpp StorageInterface/Searcher.cpp

liblux_includedir = $(includedir)/lux
liblux_include_HEADERS = index.h search.h lux.h types.h scoped_ptr.h luxconfig.h

liblux_document_includedir = $(liblux_includedir)/Document
liblux_document_include_HEADERS = \
    Document/Document.h Document/DocumentDefinition.h \
    Document/Field.h Document/FieldDefinition.h

liblux_langutil_includedir = $(liblux_includedir)/LangUtil
liblux_langutil_include_HEADERS = \
    LangUtil/LangUtil.h LangUtil/UTF8Util.h

liblux_lexer_includedir = $(liblux_includedir)/Lexer
liblux_lexer_include_HEADERS = \
    Lexer/Lexer.h Lexer/MecabMA.h  Lexer/Term.h

liblux_query_includedir = $(liblux_includedir)/Query
liblux_query_include_HEADERS = \
    Query/BooleanQuery.h Query/PhraseQuery.h \
    Query/Query.h Query/TermQuery.h

liblux_storage_includedir = $(liblux_includedir)/Storage
liblux_storage_include_HEADERS = \
    Storage/IndexResult.h Storage/SearchIndex.h \
	Storage/BooleanMerger.h Storage/DocStorage.h \
    Storage/InvertedIndex.h Storage/Normalizer.h \
    Storage/DocStorageHash.h Storage/InvertedIndexCore.h \
    Storage/Posting.h

liblux_storageengine_includedir = $(liblux_includedir)/StorageEngine
liblux_storageengine_include_HEADERS = \
    StorageEngine/CuriaHash.h StorageEngine/StorageEngineUtil.h \
    StorageEngine/DataUnit.h StorageEngine/QDBMDataUnit.h \
    StorageEngine/VillaBtree.h StorageEngine/DepotHash.h \
    StorageEngine/StorageEngine.h

liblux_storageinterface_includedir = $(liblux_includedir)/StorageInterface
liblux_storageinterface_include_HEADERS = \
	StorageInterface/Condition.h StorageInterface/QueryParser.h \
	StorageInterface/Searcher.h StorageInterface/Indexer.h \
	StorageInterface/Result.h StorageInterface/Sorter.h
