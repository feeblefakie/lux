INCLUDES = -I. -I./Document -I./Lexer -I./Query -I./StorageEngine \
           -I./LangUtil -I./Storage -I./StorageInterface

lib_LTLIBRARIES = liblux.la

liblux_la_SOURCES = Lexer/MecabMA.cpp Storage/InvertedIndex.cpp \
                    StorageInterface/Indexer.cpp StorageInterface/Searcher.cpp \
					StorageInterface/Engine.cpp StorageInterface/document_config_parser.cpp \
					Document/document_definition.pb.cpp Storage/vbc.h Storage/posting.h \
					Storage/Normalizer.h Storage/BooleanMerger.h

liblux_includedir = $(includedir)/lux
liblux_include_HEADERS = index.h search.h lux.h types.h luxconfig.h util.h scoped_ptr.h

liblux_document_includedir = $(liblux_includedir)/Document
liblux_document_include_HEADERS = \
    Document/Document.h Document/document_definition.pb.h \
    Document/Field.h Document/DocumentSerializer.h

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
    Storage/IndexResult.h Storage/SearchIndex.h Storage/IDManager.h \
	Storage/DocStorage.h Storage/AttrIndex.h Storage/AttrIndexArray.h \
    Storage/InvertedIndex.h Storage/DocStorageArray.h Storage/InvertedIndexCore.h

liblux_storageengine_includedir = $(liblux_includedir)/StorageEngine
liblux_storageengine_include_HEADERS = \
    StorageEngine/StorageEngineUtil.h StorageEngine/DataUnit.h \
	StorageEngine/LIODataUnit.h StorageEngine/StorageEngine.h \
	StorageEngine/LIOBtree.h StorageEngine/LIOArray.h

liblux_storageinterface_includedir = $(liblux_includedir)/StorageInterface
liblux_storageinterface_include_HEADERS = \
	StorageInterface/Condition.h StorageInterface/QueryParser.h \
	StorageInterface/Searcher.h StorageInterface/Indexer.h \
	StorageInterface/Result.h StorageInterface/Sorter.h \
	StorageInterface/Engine.h StorageInterface/Paging.h \
	StorageInterface/document_config_parser.h
