// SPDX-License-Identifier: GPL-2.0
#ifndef TEMPLATELAYOUT_H
#define TEMPLATELAYOUT_H

#include <QStringList>
#include "mainwindow.h"
#include "printoptions.h"
#include "core/statistics.h"
#include "core/qthelper.h"
#include "core/subsurface-qt/diveobjecthelper.h"
#include "core/subsurface-qt/cylinderobjecthelper.h" // TODO: remove once grantlee supports Q_GADGET objects

int getTotalWork(const print_options &printOptions);
void find_all_templates();
void set_bundled_templates_as_read_only();
void copy_bundled_templates(QString src, QString dst, QStringList *templateBackupList);

enum token_t {LITERAL, FORSTART, FORSTOP, BLOCKSTART, BLOCKSTOP, IFSTART, IFSTOP, PARSERERROR};

struct token {
	enum token_t type;
	QString contents;
};

extern QList<QString> grantlee_templates, grantlee_statistics_templates;

struct YearInfo {
	stats_t *year;
};

class TemplateLayout : public QObject {
	Q_OBJECT
public:
	TemplateLayout(const print_options &printOptions, const template_options &templateOptions);
	QString generate();
	QString generateStatistics();
	static QString readTemplate(QString template_name);
	static void writeTemplate(QString template_name, QString grantlee_template);

private:
	struct State {
		QList<DiveObjectHelperGrantlee> dives;
		QList<YearInfo> years;
		QMap<QString, QString> types;
		int forloopiterator = -1;
		const DiveObjectHelperGrantlee *currentDive = nullptr;
		const YearInfo *currentYear = nullptr;
		const QString *currentCylinder = nullptr;
		const CylinderObjectHelper *currentCylinderObject = nullptr;
	};
	const print_options &printOptions;
	const template_options &templateOptions;
	QList<token> lexer(QString input);
	void parser(QList<token> tokenList, int from, int to, QTextStream &out, State &state);
	template<typename V, typename T>
	void parser_for(QList<token> tokenList, int from, int to, QTextStream &out, State &state, const V &data, const T *&act);
	QVariant getValue(QString list, QString property, const State &state);
	QString translate(QString s, State &state);

signals:
	void progressUpdated(int value);
};

#endif
