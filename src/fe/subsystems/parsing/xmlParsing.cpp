#include "fe/subsystems/parsing/xmlParsing.hpp"
#include <fstream>
#include <sstream>

rapidxml::xml_document<> *fe::loadFile(const char *filepath, std::string &content)
    {
        rapidxml::xml_document<> *doc = new rapidxml::xml_document<>();

        std::ifstream in(filepath);
        std::stringstream buffer;
        buffer << in.rdbuf();
        in.close();
        content = buffer.str();

        doc->parse<0>(&content[0]);

        return doc;
    }

void fe::prettyPrint(rapidxml::xml_document<> *document, std::ostream &out)
    {
        // Todo
    }

void fe::saveFile(const char *filepath, rapidxml::xml_document<> *document)
    {
        std::ofstream out(filepath);
        prettyPrint(document, out);
        out.close();

        freeDocument(document);
    }

void fe::freeDocument(rapidxml::xml_document<> *document)
    {
        delete document;
    }
