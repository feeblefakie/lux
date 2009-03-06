INCLUDES = -I. -I./document -I./lexer -I./query -I./storage \
           -I./lang -I./index -I./api

lib_LTLIBRARIES = liblux.la

liblux_la_SOURCES = lexer/mecab_ma.cpp lexer/term.cpp \
					query/term_query.cpp query/phrase_query.cpp query/boolean_query.cpp \
					lang/utf8_util.cpp \
					index/attr_index_array.cpp index/doc_storage_array.cpp \
					index/id_manager.cpp index/inverted_index.cpp \
                    api/indexer.cpp api/searcher.cpp api/engine.cpp \
					api/query_parser.cpp api/document_config_parser.cpp \
					api/paging.cpp api/result.cpp api/condition.cpp \
					document/document.cpp document/document_serializer.cpp \
					document/document_definition.pb.cpp index/vbc.h index/posting.h \
					index/normalizer.h index/boolean_merger.h

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
