#ifndef DOCUMENT_REPOSITORY_H
#define DOCUMENT_REPOSITORY_H

#include <cstdint>
#include <unordered_map>

namespace Indexer
{

typedef std::uint32_t docID;

typedef std::unordered_map<
    std::string,
    docID
> DocumentRepository_T;

class DocumentRepository
{
    public:
        static docID getDocID(const std::string & document_path);
        static const DocumentRepository_T & data();

    private:
        static DocumentRepository_T __repository;
        static docID next_doc_id;
        static docID getNextDocID();
};

} // Namespace Indexer

#endif
