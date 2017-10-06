// xmlParsing.hpp
// A set of functions to assist with XML parsing
#pragma once
#include <rapidxml.hpp>
#include <ostream>

namespace fe
    {
        // Allocated a new XML document off the internal buffer and reads the data from the filepath
        rapidxml::xml_document<> *loadFile(const char *filepath);

        // Pretty Prints the document to the ostream
        void prettyPrint(rapidxml::xml_document<> *document, std::ostream &out);

        // Saves the XML document and frees the documents memory
        void saveFile(const char *filepath, rapidxml::xml_document<> *document);
        // Frees the XML document
        void freeDocument(rapidxml::xml_document<> *document);
    }