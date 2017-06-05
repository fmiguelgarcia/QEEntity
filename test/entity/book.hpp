#pragma once
#include <QObject>
#include "entity/chapter.hpp"
#include <qe/entity/OneToManyAdapter.hpp>
#include <vector>

class Book 
	: public QObject
{
	// Properties
	Q_OBJECT 
	Q_PROPERTY( int id MEMBER id)
	Q_PROPERTY( QString title MEMBER title)
	Q_PROPERTY( QString author MEMBER author)
	Q_PROPERTY( int pages MEMBER pages)
	Q_PROPERTY( QByteArray binSignature MEMBER binSignature)
	Q_PROPERTY( int entityDisable MEMBER entityDisable )
	Q_PROPERTY( QStringList footNotes MEMBER footNotes)
	Q_PROPERTY( QVariantList chapters READ chaptersAdapter WRITE chaptersAdapter)

	// Annotations
	Q_CLASSINFO( "class", "@qe.model.name=book")
	Q_CLASSINFO( "id", "@qe.entity.isAutoIncrementable=true @qe.entity.name=bid")
	Q_CLASSINFO( "title", "@qe.entity.maxLength=256 @qe.entity.isNullable=false")
	Q_CLASSINFO( "author", "@qe.entity.maxLength=512")
	Q_CLASSINFO( "pages", "@qe.entity.maxLength=512")
	Q_CLASSINFO( "entityDisable", "@qe.entity.isEnabled=false")
	Q_CLASSINFO( "chapters", "@qe.entity.mapping.entity=Chapter @qe.entity.mapping.type=OneToMany")
	
	public:
		int id;
		QString title;
		QString author;
		int pages;
		QByteArray binSignature;
		int entityDisable;
		QStringList footNotes;

		std::vector<Chapter> chapters;
		qe::entity::OneToManyAdapter< decltype(chapters)> chaptersAdapter{ chapters};
};
