#include <DocumentRepository.h>

namespace Indexer
{

DocumentRepository_T DocumentRepository::__repository;
docID DocumentRepository::next_doc_id = 1;

docID DocumentRepository::getDocID(const std::string & document_path)
{
    if ( DocumentRepository::__repository.count(document_path) == 0 ) {
        DocumentRepository::__repository[document_path] = DocumentRepository::getNextDocID();
    }

    return DocumentRepository::__repository[document_path];
}

const DocumentRepository_T &
DocumentRepository::data()
{
    return DocumentRepository::__repository;
}

docID DocumentRepository::getNextDocID()
{
    return DocumentRepository::next_doc_id++;
}

} // Namespace Indexer
