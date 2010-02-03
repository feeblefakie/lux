INCLUDES = -I. -I./document -I./lexer -I./query -I./storage \
           -I./lang -I./index -I./api

lib_LTLIBRARIES = liblux.la

liblux_la_SOURCES = lexer/lexer.h lexer/mecab_ma.h lexer/mecab_ma.cpp lexer/term.h lexer/term.cpp \
					query/query.h query/term_query.h query/term_query.cpp query/phrase_query.h \
					query/phrase_query.cpp query/boolean_query.h query/boolean_query.cpp \
					lang/lang_util.h lang/utf8_util.h lang/utf8_util.cpp \
					index/attr_index.h index/attr_index_array.h index/attr_index_array.cpp \
					index/doc_storage.h index/doc_storage_array.h index/doc_storage_array.cpp \
					index/id_manager.h index/id_manager.cpp index/search_index.h \
					index/inverted_index.h index/inverted_index.cpp index/inverted_index_core.h \
					index/normalizer.h index/boolean_merger.h index/vbc.h index/posting.h \
                    api/indexer.cpp api/searcher.cpp api/engine.cpp api/engine-impl.h \
					api/query_parser.h api/query_parser.cpp api/document_config_parser.cpp \
					api/paging.cpp api/result.cpp api/condition.cpp api/sorter.h \
					document/document.cpp document/document_definition.pb.cpp \
					document/document_serializer.h document/document_serializer.cpp \
					storage/storage_engine.h storage/data_unit.h \
					storage/luxio_btree.h storage/luxio_btree.cpp \
					storage/luxio_array.h storage/luxio_array.cpp \
					storage/luxio_data_unit.h storage/luxio_data_unit.cpp \
					util.h scoped_ptr.h

liblux_includedir = $(includedir)/lux
liblux_include_HEADERS = index.h search.h lux.h types.h luxconfig.h

liblux_document_includedir = $(liblux_includedir)/document
liblux_document_include_HEADERS = \
    document/document.h document/field.h document/document_definition.pb.h

liblux_index_includedir = $(liblux_includedir)/index
liblux_index_include_HEADERS = \
    index/index_result.h

liblux_api_includedir = $(liblux_includedir)/api
liblux_api_include_HEADERS = \
	api/condition.h api/searcher.h api/indexer.h \
	api/result.h api/engine.h api/paging.h \
	api/document_config_parser.h

EXTRA_DIST = \
	dist/Makefile dist/*.cpp dist/*.h dist/*.proto \
	document/document_definition.proto
