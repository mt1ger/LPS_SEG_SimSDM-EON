#include "txtParse.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string>
string_split(const string &str_in, const string &delimiter)
{
  size_t         head = 0, pos = 0;
  string         word;
  vector<string> words;

  pos = str_in.find(delimiter, head);
  while(pos != str_in.npos)
  {
    word = str_in.substr(head, pos - head);
    head = pos + delimiter.size();
    pos  = str_in.find(delimiter, head);
    words.push_back(word);
  }
  word = str_in.substr(head, str_in.size() - head);
  words.push_back(word);
  return words;
}

void
txtParse::initialization(Metric *metric_ptr)
{
  for(int i = 0; i < potentialCores.size(); i++)
  {
    for(int k = 0; k < potentialErlangs.size(); k++)
    {
      metric_ptr->array_third.push_back(potentialCores[i]);
      metric_ptr->array_third.push_back(potentialErlangs[k]);
      for(int i = 0; i < potentialSeeds.size(); i++)
      {
        metric_ptr->array_third.push_back(-1);
      }
      metric_ptr->array_sec.push_back(metric_ptr->array_third);
      metric_ptr->array_third.clear();
    }
    metric_ptr->array.push_back(metric_ptr->array_sec);
    metric_ptr->array_sec.clear();
  }
}

void
txtParse::extraction(const string &fileContent, Metric *metric_ptr,
                     string &core, string &erlang, string &seed)
{

  int coreCnt, erlangCnt, seedCnt;

  metric_ptr->dataString = fileContent;
  metric_ptr->dataNumber = stof(metric_ptr->dataString);
  double temp_core       = stof(core);
  double temp_erlang     = stof(erlang);
  double temp_seed       = stof(seed);

  int intCore = floor(temp_core + 0.005);
  for(coreCnt = 0; coreCnt < potentialCores.size(); coreCnt++)
  {
    if(intCore == potentialCores[coreCnt])
    {
      break;
    }
  }

  int normErlang = floor(temp_erlang + 0.005) / potentialCores[coreCnt];
  for(erlangCnt = 0; erlangCnt < potentialErlangs.size(); erlangCnt++)
  {
    if(normErlang == potentialErlangs[erlangCnt])
    {
      break;
    }
  }

  int intSeed = floor(temp_seed + 0.005);
  for(seedCnt = 0; seedCnt < potentialSeeds.size(); seedCnt++)
  {
    if(intSeed == potentialSeeds[seedCnt])
      break;
  }

  metric_ptr->array[coreCnt][erlangCnt].at(seedCnt + 2)
      = metric_ptr->dataNumber;
}

void
txtParse::avg_data(Metric *metric_ptr)
{
  int cnt = metric_ptr->array[0][0].size() - 2;
  for(int i = 0; i < potentialCores.size(); i++)
  {
    for(int j = 0; j < potentialErlangs.size(); j++)
    {
      double tempData = 0;

      for(int k = 2; k < 2 + cnt; k++)
      {
        tempData += metric_ptr->array[i][j][k];
      }

      tempData = tempData / cnt;

      metric_ptr->array[i][j].push_back(tempData);
    }
  }
}

void
txtParse::write_to_files(Metric *metric_ptr)
{
  metric_ptr->fileName = path + iFileName + "_" + metric_ptr->name + ".csv";

  ofstream outputFile(metric_ptr->fileName);

  string strAllSeeds;
  for(int i = 0; i < potentialSeeds.size(); i++)
  {
    strAllSeeds = strAllSeeds + to_string(potentialSeeds[i]) + ',';
  }
  strAllSeeds = strAllSeeds + '\n';
  outputFile << strAllSeeds;

  for(int i = 0; i < metric_ptr->array.size(); i++)
  {
    for(int j = 0; j < metric_ptr->array[i].size(); j++)
    {
      string strData;

      for(int k = 0; k < metric_ptr->array[i][j].size(); k++)
      {
        strData = strData + to_string(metric_ptr->array[i][j][k]) + ',';
      }
      strData = strData + '\n';
      outputFile << strData;
    }
  }

  outputFile.close();
}

void
txtParse::parse()
{

  Metric BP("BP"); // Blocking probability
  Metric NoT(
      "NoT"); // Maximum number of transponders that are simultaneously used
  Metric CpR("CpR");         // Number of Cores per request
  Metric HTpR("HTpR");       // Holding time per request
  Metric TpR("TpR");         // Number of Transponders per request
  Metric LPSpR("LPSpR");     // Number of LPS per request
  Metric AvgIFpR("AvgIFpR"); // Average internal fragmentation per request
  Metric AvgEFpR("AvgEFpR"); // Average external fragmentation per request
  Metric AvgHFpR("AvgHFpR"); // Average hybrid fragmentation per request
  Metric Numof400SC6(
      "Numof400SC6"); // Number of 400 Gb/s with 64QAM super channel used
  Metric Numof400SC4(
      "Numof400SC4"); // Number of 400 Gb/s with 16QAM super channel used
  Metric Numof400SC2(
      "Numof400SC2"); // Number of 400 Gb/s with QPSK super channel used
  Metric Numof200SC6(
      "Numof200SC6"); // Number of 200 Gb/s with 64QAM super channel used
  Metric Numof200SC4(
      "Numof200SC4"); // Number of 200 Gb/s with 16QAM super channel used
  Metric Numof200SC2(
      "Numof200SC2"); // Number of 200 Gb/s with QPSK super channel used
  Metric Numof100SC6(
      "Numof100SC6"); // Number of 100 Gb/s with 64QAM super channel used
  Metric Numof100SC4(
      "Numof100SC4"); // Number of 100 Gb/s with 16QAM super channel used
  Metric Numof100SC2(
      "Numof100SC2"); // Number of 100 Gb/s with QPSK super channel used
  Metric Numof50SC6(
      "Numof50SC6"); // Number of 50 Gb/s with 64QAM super channel used
  Metric Numof50SC4(
      "Numof50SC4"); // Number of 50 Gb/s with 16QAM super channel used
  Metric Numof50SC2(
      "Numof50SC2"); // Number of 50 Gb/s with QPSK super channel used
  Metric Numof25SC(
      "Numof25SC");            // Number of 25 Gb/s with QPSK super channel used
  Metric Block400("Block400"); // Number of blocked 400 Gb/s request
  Metric Block100("Block100"); // Number of blocked 100 Gb/s request
  Metric Block40("Block40");   // Number of blocked 40 Gb/s request
  Metric BlockAR("BlockAR");   // Number of blocked AR request
  Metric BlockIR("BlockIR");   // Number of blocked IR request
  Metric Block400AR("Block400AR"); // Number of blocked 400 Gb/s AR request
  Metric Block400IR("Block400IR"); // Number of blocked 400 Gb/s IR request
  Metric Block100AR("Block100AR"); // Number of blocked 100 Gb/s AR request
  Metric Block100IR("Block100IR"); // Number of blocked 100 Gb/s IR request
  Metric Block40AR("Block40AR");   // Number of blocked 40 Gb/s AR request
  Metric Block40IR("Block40IR");   // Number of blocked 40 Gb/s IR request

  iFileFullPath = path + iFileName + ".txt";
  ifstream inputFile(iFileFullPath);

  string core, erlang, seed;

  /*** Initialization:  {core, erlang, data1, data2, data3, ...} ***/
  initialization(&BP);
  initialization(&NoT);
  initialization(&CpR);
  initialization(&HTpR);
  initialization(&TpR);
  initialization(&LPSpR);
  initialization(&AvgIFpR);
  initialization(&AvgEFpR);
  initialization(&AvgHFpR);
  initialization(&Numof400SC6);
  initialization(&Numof400SC4);
  initialization(&Numof400SC2);
  initialization(&Numof200SC6);
  initialization(&Numof200SC4);
  initialization(&Numof200SC2);
  initialization(&Numof100SC6);
  initialization(&Numof100SC4);
  initialization(&Numof100SC2);
  initialization(&Numof50SC6);
  initialization(&Numof50SC4);
  initialization(&Numof50SC2);
  initialization(&Numof25SC);
  initialization(&Block400);
  initialization(&Block100);
  initialization(&Block40);
  initialization(&BlockAR);
  initialization(&BlockIR);
  initialization(&Block400AR);
  initialization(&Block400IR);
  initialization(&Block100AR);
  initialization(&Block100IR);
  initialization(&Block40AR);
  initialization(&Block40IR);

  /*** Extract data from file ***/
  string str_in;
  while(inputFile >> str_in)
  {
    vector<string> arr_strings = string_split(str_in, ",");

    core   = arr_strings[0];
    erlang = arr_strings[1];
    seed   = arr_strings[2];

    extraction(arr_strings[3], &BP, core, erlang, seed);
    extraction(arr_strings[4], &NoT, core, erlang, seed);
    extraction(arr_strings[5], &CpR, core, erlang, seed);
    extraction(arr_strings[6], &HTpR, core, erlang, seed);
    extraction(arr_strings[7], &TpR, core, erlang, seed);
    extraction(arr_strings[8], &LPSpR, core, erlang, seed);
    extraction(arr_strings[9], &AvgIFpR, core, erlang, seed);
    extraction(arr_strings[10], &AvgEFpR, core, erlang, seed);
    extraction(arr_strings[11], &AvgHFpR, core, erlang, seed);
    extraction(arr_strings[12], &Numof400SC6, core, erlang, seed);
    extraction(arr_strings[13], &Numof400SC4, core, erlang, seed);
    extraction(arr_strings[14], &Numof400SC2, core, erlang, seed);
    extraction(arr_strings[15], &Numof200SC6, core, erlang, seed);
    extraction(arr_strings[16], &Numof200SC4, core, erlang, seed);
    extraction(arr_strings[17], &Numof200SC2, core, erlang, seed);
    extraction(arr_strings[18], &Numof100SC6, core, erlang, seed);
    extraction(arr_strings[19], &Numof100SC4, core, erlang, seed);
    extraction(arr_strings[20], &Numof100SC2, core, erlang, seed);
    extraction(arr_strings[21], &Numof50SC6, core, erlang, seed);
    extraction(arr_strings[22], &Numof50SC4, core, erlang, seed);
    extraction(arr_strings[23], &Numof50SC2, core, erlang, seed);
    extraction(arr_strings[24], &Numof25SC, core, erlang, seed);
    extraction(arr_strings[25], &Block400, core, erlang, seed);
    extraction(arr_strings[26], &Block100, core, erlang, seed);
    extraction(arr_strings[27], &Block40, core, erlang, seed);
    extraction(arr_strings[28], &BlockAR, core, erlang, seed);
    extraction(arr_strings[29], &BlockIR, core, erlang, seed);
    extraction(arr_strings[30], &Block400AR, core, erlang, seed);
    extraction(arr_strings[31], &Block400IR, core, erlang, seed);
    extraction(arr_strings[32], &Block100AR, core, erlang, seed);
    extraction(arr_strings[33], &Block100IR, core, erlang, seed);
    extraction(arr_strings[34], &Block40AR, core, erlang, seed);
    extraction(arr_strings[35], &Block40IR, core, erlang, seed);
  }

  /*** Print what is in BP.array ***/
  // for(int i = 0; i < BP.array.size(); i++)
  // {
  //   for(int j = 0; j < BP.array[i].size(); j++)
  //   {
  //     for(int k = 0; k < BP.array[i][j].size(); k++)
  //     {
  //
  //       cout << BP.array[i][j][k] << ' ';
  //     }
  //     cout << endl;
  //   }
  // }

  /*** Compute avg of data ***/
  avg_data(&BP);
  avg_data(&NoT);
  avg_data(&CpR);
  avg_data(&HTpR);
  avg_data(&TpR);
  avg_data(&LPSpR);
  avg_data(&AvgIFpR);
  avg_data(&AvgEFpR);
  avg_data(&AvgHFpR);
  avg_data(&Numof400SC6);
  avg_data(&Numof400SC4);
  avg_data(&Numof400SC2);
  avg_data(&Numof200SC6);
  avg_data(&Numof200SC4);
  avg_data(&Numof200SC2);
  avg_data(&Numof100SC6);
  avg_data(&Numof100SC4);
  avg_data(&Numof100SC2);
  avg_data(&Numof50SC6);
  avg_data(&Numof50SC4);
  avg_data(&Numof50SC2);
  avg_data(&Numof25SC);
  avg_data(&Block400);
  avg_data(&Block100);
  avg_data(&Block40);
  avg_data(&BlockAR);
  avg_data(&BlockIR);
  avg_data(&Block400AR);
  avg_data(&Block400IR);
  avg_data(&Block100AR);
  avg_data(&Block100IR);
  avg_data(&Block40AR);
  avg_data(&Block40IR);

  // *** Print what is in BParray
  // for(int i = 0; i < BParray.size(); i++)
  // {
  //   for(int j = 0; j < BParray[i].size(); j++)
  //   {
  //     for(int k = 0; k < BParray[i][j].size(); k++)
  //     {
  //
  //       cout << BParray[i][j][k] << ' ';
  //     }
  //   cout << endl;
  //   }
  // }

  /*** Write data to the associated file ***/
  write_to_files(&BP);
  write_to_files(&NoT);
  write_to_files(&CpR);
  write_to_files(&HTpR);
  write_to_files(&TpR);
  write_to_files(&LPSpR);
  write_to_files(&AvgIFpR);
  write_to_files(&AvgEFpR);
  write_to_files(&AvgHFpR);
  write_to_files(&Numof400SC6);
  write_to_files(&Numof400SC4);
  write_to_files(&Numof400SC2);
  write_to_files(&Numof200SC6);
  write_to_files(&Numof200SC4);
  write_to_files(&Numof200SC2);
  write_to_files(&Numof100SC6);
  write_to_files(&Numof100SC4);
  write_to_files(&Numof100SC2);
  write_to_files(&Numof50SC6);
  write_to_files(&Numof50SC4);
  write_to_files(&Numof50SC2);
  write_to_files(&Numof25SC);
  write_to_files(&Block400);
  write_to_files(&Block100);
  write_to_files(&Block40);
  write_to_files(&BlockAR);
  write_to_files(&BlockIR);
  write_to_files(&Block400AR);
  write_to_files(&Block400IR);
  write_to_files(&Block100AR);
  write_to_files(&Block100IR);
  write_to_files(&Block40AR);
  write_to_files(&Block40IR);
}
