#ifndef CARDREADER_CARDREADERPLUGIN_H
#define CARDREADER_CARDREADERPLUGIN_H

#include "../cardreaderpluginglobal.h"

#include <qf/core/utils.h>
#include <qf/qmlwidgets/framework/plugin.h>

#include <QQmlListProperty>

namespace CardReader {

class CardChecker;
class ReadCard;
class CheckedCard;

class CARDREADERPLUGIN_DECL_EXPORT CardReaderPlugin : public qf::qmlwidgets::framework::Plugin
{
	Q_OBJECT
	Q_PROPERTY(QQmlListProperty<CardReader::CardChecker> cardCheckers READ cardCheckersListProperty)
	Q_PROPERTY(int currentCardCheckerIndex READ currentCardCheckerIndex WRITE setCurrentCardCheckerIndex NOTIFY currentCardCheckerIndexChanged)
private:
	typedef qf::qmlwidgets::framework::Plugin Super;
public:
	CardReaderPlugin(QObject *parent = nullptr);

	static const char* DBEVENTDOMAIN_CARDREADER_CARDREAD;
	static const QLatin1String SETTINGS_PREFIX;

	QF_PROPERTY_IMPL2(int, c, C, urrentCardCheckerIndex, -1)

	const QList<CardReader::CardChecker*>& cardCheckers() {return m_cardCheckers;}
	CardReader::CardChecker* currentCardChecker();

	Q_INVOKABLE QString settingsPrefix();

	int currentStageId();
	int findRunId(int si_id);
	CheckedCard checkCard(int card_id, int run_id = 0);
	CheckedCard checkCard(const ReadCard &read_card);
	int saveCardToSql(const ReadCard &read_card);
	//ReadCard loadCardFromSql(int card_id);
	bool updateRunLapsSql(const CheckedCard &checked_card);
private:
	void onInstalled();
	QQmlListProperty<CardChecker> cardCheckersListProperty();
private:
	QList<CardChecker*> m_cardCheckers;
};

}

#endif
