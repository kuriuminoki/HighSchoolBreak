#ifndef CSV_READER_H_INCLUDED
#define CSV_READER_H_INCLUDED

#include <vector>
#include <string>
#include <map>


std::vector<std::string> csv2vector(std::string buff);


/*
* CSVファイルを読み込む。各行をmap型で表現し、全行をvectorで保持する。
* 例：
* input.csv =
* ```
* column1, column2, column3
* a, b, c
* d, e, f
* ```
* ->
* m_data = [{column1:a, column2:b, column3:c}, {column1:d, column2:e, column3:f}]
*/
class CsvReader {
private:
	/*
	* データ
	* m_data[行番号]<カラム名, データ>
	*/
	std::vector<std::map<std::string, std::string> > m_data;

	/*
	* カラム名のリスト
	*/
	std::vector<std::string> m_columnNames;

public:
	// ファイル名を指定してCSVファイルを読み込む
	CsvReader(const char* fileName);

	/*
	* カラム名がvalueのデータを取得
	* 例：findOne("Name", "キャラ名");
	*/ 
	std::map<std::string, std::string> findOne(const char* columnName, const char* value);

	/*
	* 全データを返す
	*/
	std::vector<std::map<std::string, std::string> > getData() const;
};


#endif