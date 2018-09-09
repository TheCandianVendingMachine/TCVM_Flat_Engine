// xmlParsing.hpp
// A set of functions to assist with XML parsing
#pragma once
#include "../../flatEngineExport.hpp"

#include <rapidxml.hpp>
#include <ostream>

namespace fe
    {
        // Allocated a new XML document off the internal buffer and reads the data from the filepath
        FLAT_ENGINE_API rapidxml::xml_document<> *loadFile(const char *filepath, std::string &content);

        // Pretty Prints the document to the ostream
        FLAT_ENGINE_API void prettyPrint(rapidxml::xml_document<> *document, std::ostream &out);

        // Saves the XML document and frees the documents memory
        FLAT_ENGINE_API void saveFile(const char *filepath, rapidxml::xml_document<> *document);
        // Frees the XML document
        FLAT_ENGINE_API void freeDocument(rapidxml::xml_document<> *document);
    }