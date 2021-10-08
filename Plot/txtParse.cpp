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
  Metric maxTransponders(
      "maxTransponders"); // Maximum number of transponders that are
                               // simultaneously used
  Metric totalTransponders(
      "totalTransponders");  // The total number of transponders used for this
                             // simulation run
  Metric CpR("CpR");         // Number of Cores per request
  Metric avgHoldingTime("avgHoldingTime");       // Holding time per request
  Metric avgTransponders("avgTransponders");         // Number of Transponders per request
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
  Metric totalBlocks_AR("totalBlocks_AR"); // Number of blocked AR request
  Metric totalBlocks_IR("totalBlocks_IR"); // Number of blocked IR request
  Metric totalBlocks_400(
      "totalBlocks_400"); // Number of blocked 400 Gb/s request
  Metric totalBlocks_400AR(
      "totalBlocks_400AR"); // Number of blocked 400 Gb/s AR request
  Metric totalBlocks_400IR(
      "totalBlocks_400IR"); // Number of blocked 400 Gb/s IR request
  Metric totalBlocks_100(
      "totalBlocks_100"); // Number of blocked 100 Gb/s request
  Metric totalBlocks_100AR(
      "totalBlocks_100AR"); // Number of blocked 100 Gb/s AR request
  Metric totalBlocks_100IR(
      "totalBlocks_100IR"); // Number of blocked 100 Gb/s IR request
  Metric totalBlocks_40("totalBlocks_40"); // Number of blocked 40 Gb/s request
  Metric totalBlocks_40AR(
      "totalBlocks_40AR"); // Number of blocked 40 Gb/s AR request
  Metric totalBlocks_40IR(
      "totalBlocks_40IR"); // Number of blocked 40 Gb/s IR request
  Metric totalTransponders_AR(
      "totalTransponders_AR"); // Number of blocked AR request
  Metric totalTransponders_IR(
      "totalTransponders_IR"); // Number of blocked IR request
  Metric totalTransponders_400(
      "totalTransponders_400"); // Number of blocked 400 Gb/s request
  Metric totalTransponders_400AR(
      "totalTransponders_400AR"); // Number of blocked 400 Gb/s AR request
  Metric totalTransponders_400IR(
      "totalTransponders_400IR"); // Number of blocked 400 Gb/s IR request
  Metric totalTransponders_100(
      "totalTransponders_100"); // Number of blocked 100 Gb/s request
  Metric totalTransponders_100AR(
      "totalTransponders_100AR"); // Number of blocked 100 Gb/s AR request
  Metric totalTransponders_100IR(
      "totalTransponders_100IR"); // Number of blocked 100 Gb/s IR request
  Metric totalTransponders_40(
      "totalTransponders_40"); // Number of blocked 40 Gb/s request
  Metric totalTransponders_40AR(
      "totalTransponders_40AR"); // Number of blocked 40 Gb/s AR request
  Metric totalTransponders_40IR(
      "totalTransponders_40IR");   // Number of blocked 40 Gb/s IR request
  Metric avgTransponders_AR("avgTransponders_AR");         // Number of blocked AR request
  Metric avgTransponders_IR("avgTransponders_IR");         // Number of blocked IR request
  Metric avgTransponders_400("avgTransponders_400");       // Number of blocked 400 Gb/s request
  Metric avgTransponders_400AR("avgTransponders_400AR");   // Number of blocked 400 Gb/s AR request
  Metric avgTransponders_400IR("avgTransponders_400IR");   // Number of blocked 400 Gb/s IR request
  Metric avgTransponders_100("avgTransponders_100");       // Number of blocked 100 Gb/s request
  Metric avgTransponders_100AR("avgTransponders_100AR");   // Number of blocked 100 Gb/s AR request
  Metric avgTransponders_100IR("avgTransponders_100IR");   // Number of blocked 100 Gb/s IR request
  Metric avgTransponders_40("avgTransponders_40");         // Number of blocked 40 Gb/s request
  Metric avgTransponders_40AR("avgTransponders_40AR");     // Number of blocked 40 Gb/s AR request
  Metric avgTransponders_40IR("avgTransponders_40IR");     // Number of blocked 40 Gb/s IR request
  Metric TotalLPSs_AR("TotalLPSs_AR");  // Number of LPSs for AR requests
  Metric TotalLPSs_IR("TotalLPSs_IR");  // Number of LPSs for IR requests
  Metric TotalLPSs_400("TotalLPSs_400"); // Number of LPSs for 400 Gb/s requests
  Metric TotalLPSs_400AR(
      "TotalLPSs_400AR"); // Number of LPSs for 400 Gb/s AR requests
  Metric TotalLPSs_400IR(
      "TotalLPSs_400IR"); // Number of LPSs for 400 Gb/s IR requests
  Metric TotalLPSs_100("TotalLPSs_100"); // Number of LPSs for 100 Gb/s requests
  Metric TotalLPSs_100AR(
      "TotalLPSs_100AR"); // Number of LPSs for 100 Gb/s AR requests
  Metric TotalLPSs_100IR(
      "TotalLPSs_100IR"); // Number of LPSs for 100 Gb/s IR requests
  Metric TotalLPSs_40("TotalLPSs_40"); // Number of LPSs for 40 Gb/s requests
  Metric TotalLPSs_40AR(
      "TotalLPSs_40AR"); // Number of LPSs for 40 Gb/s AR requests
  Metric TotalLPSs_40IR(
      "TotalLPSs_40IR"); // Number of LPSs for 40 Gb/s IR requests


  iFileFullPath = path + iFileName + ".txt";
  ifstream inputFile(iFileFullPath);

  string core, erlang, seed;

  /*** Initialization:  {core, erlang, data1, data2, data3, ...} ***/
  initialization(&BP);
  initialization(&maxTransponders);
  initialization(&totalTransponders);
  initialization(&CpR);
  initialization(&avgHoldingTime);
  initialization(&avgTransponders);
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
  initialization(&totalBlocks_AR);
  initialization(&totalBlocks_IR);
  initialization(&totalBlocks_400);
  initialization(&totalBlocks_400AR);
  initialization(&totalBlocks_400IR);
  initialization(&totalBlocks_100);
  initialization(&totalBlocks_100AR);
  initialization(&totalBlocks_100IR);
  initialization(&totalBlocks_40);
  initialization(&totalBlocks_40AR);
  initialization(&totalBlocks_40IR);
  initialization(&totalTransponders_AR);
  initialization(&totalTransponders_IR);
  initialization(&totalTransponders_400);
  initialization(&totalTransponders_400AR);
  initialization(&totalTransponders_400IR);
  initialization(&totalTransponders_100);
  initialization(&totalTransponders_100AR);
  initialization(&totalTransponders_100IR);
  initialization(&totalTransponders_40);
  initialization(&totalTransponders_40AR);
  initialization(&totalTransponders_40IR);
  initialization(&avgTransponders_AR);
  initialization(&avgTransponders_IR);
  initialization(&avgTransponders_400);
  initialization(&avgTransponders_400AR);
  initialization(&avgTransponders_400IR);
  initialization(&avgTransponders_100);
  initialization(&avgTransponders_100AR);
  initialization(&avgTransponders_100IR);
  initialization(&avgTransponders_40);
  initialization(&avgTransponders_40AR);
  initialization(&avgTransponders_40IR);
  initialization(&TotalLPSs_AR);
  initialization(&TotalLPSs_IR);
  initialization(&TotalLPSs_400);
  initialization(&TotalLPSs_400AR);
  initialization(&TotalLPSs_400IR);
  initialization(&TotalLPSs_100);
  initialization(&TotalLPSs_100AR);
  initialization(&TotalLPSs_100IR);
  initialization(&TotalLPSs_40);
  initialization(&TotalLPSs_40AR);
  initialization(&TotalLPSs_40IR);

  /*** Extract data from file ***/
  string str_in;
  while(inputFile >> str_in)
  {
    vector<string> arr_strings = string_split(str_in, ",");

    core   = arr_strings[0];
    erlang = arr_strings[1];
    seed   = arr_strings[2];

    extraction(arr_strings[3], &BP, core, erlang, seed);
    extraction(arr_strings[4], &maxTransponders, core, erlang, seed);
    extraction(arr_strings[5], &totalTransponders, core, erlang, seed);
    extraction(arr_strings[6], &CpR, core, erlang, seed);
    extraction(arr_strings[7], &avgHoldingTime, core, erlang, seed);
    extraction(arr_strings[8], &avgTransponders, core, erlang, seed);
    extraction(arr_strings[9], &LPSpR, core, erlang, seed);
    extraction(arr_strings[10], &AvgIFpR, core, erlang, seed);
    extraction(arr_strings[11], &AvgEFpR, core, erlang, seed);
    extraction(arr_strings[12], &AvgHFpR, core, erlang, seed);
    extraction(arr_strings[13], &Numof400SC6, core, erlang, seed);
    extraction(arr_strings[14], &Numof400SC4, core, erlang, seed);
    extraction(arr_strings[15], &Numof400SC2, core, erlang, seed);
    extraction(arr_strings[16], &Numof200SC6, core, erlang, seed);
    extraction(arr_strings[17], &Numof200SC4, core, erlang, seed);
    extraction(arr_strings[18], &Numof200SC2, core, erlang, seed);
    extraction(arr_strings[19], &Numof100SC6, core, erlang, seed);
    extraction(arr_strings[20], &Numof100SC4, core, erlang, seed);
    extraction(arr_strings[21], &Numof100SC2, core, erlang, seed);
    extraction(arr_strings[22], &Numof50SC6, core, erlang, seed);
    extraction(arr_strings[23], &Numof50SC4, core, erlang, seed);
    extraction(arr_strings[24], &Numof50SC2, core, erlang, seed);
    extraction(arr_strings[25], &Numof25SC, core, erlang, seed);
    extraction(arr_strings[26], &totalBlocks_AR, core, erlang, seed);
    extraction(arr_strings[27], &totalBlocks_IR, core, erlang, seed);
    extraction(arr_strings[28], &totalBlocks_400, core, erlang, seed);
    extraction(arr_strings[29], &totalBlocks_400AR, core, erlang, seed);
    extraction(arr_strings[30], &totalBlocks_400IR, core, erlang, seed);
    extraction(arr_strings[31], &totalBlocks_100, core, erlang, seed);
    extraction(arr_strings[32], &totalBlocks_100AR, core, erlang, seed);
    extraction(arr_strings[33], &totalBlocks_100IR, core, erlang, seed);
    extraction(arr_strings[34], &totalBlocks_40, core, erlang, seed);
    extraction(arr_strings[35], &totalBlocks_40AR, core, erlang, seed);
    extraction(arr_strings[36], &totalBlocks_40IR, core, erlang, seed);
    extraction(arr_strings[37], &totalTransponders_AR, core, erlang, seed);
    extraction(arr_strings[38], &totalTransponders_IR, core, erlang, seed);
    extraction(arr_strings[39], &totalTransponders_400, core, erlang, seed);
    extraction(arr_strings[40], &totalTransponders_400AR, core, erlang, seed);
    extraction(arr_strings[41], &totalTransponders_400IR, core, erlang, seed);
    extraction(arr_strings[42], &totalTransponders_100, core, erlang, seed);
    extraction(arr_strings[43], &totalTransponders_100AR, core, erlang, seed);
    extraction(arr_strings[44], &totalTransponders_100IR, core, erlang, seed);
    extraction(arr_strings[45], &totalTransponders_40, core, erlang, seed);
    extraction(arr_strings[46], &totalTransponders_40AR, core, erlang, seed);
    extraction(arr_strings[47], &totalTransponders_40IR, core, erlang, seed);
    extraction(arr_strings[48], &avgTransponders_AR, core, erlang, seed);
    extraction(arr_strings[49], &avgTransponders_IR, core, erlang, seed);
    extraction(arr_strings[50], &avgTransponders_400, core, erlang, seed);
    extraction(arr_strings[51], &avgTransponders_400AR, core, erlang, seed);
    extraction(arr_strings[52], &avgTransponders_400IR, core, erlang, seed);
    extraction(arr_strings[53], &avgTransponders_100, core, erlang, seed);
    extraction(arr_strings[54], &avgTransponders_100AR, core, erlang, seed);
    extraction(arr_strings[55], &avgTransponders_100IR, core, erlang, seed);
    extraction(arr_strings[56], &avgTransponders_40, core, erlang, seed);
    extraction(arr_strings[57], &avgTransponders_40AR, core, erlang, seed);
    extraction(arr_strings[58], &avgTransponders_40IR, core, erlang, seed);
    extraction(arr_strings[59], &TotalLPSs_AR, core, erlang, seed);
    extraction(arr_strings[60], &TotalLPSs_IR, core, erlang, seed);
    extraction(arr_strings[61], &TotalLPSs_400, core, erlang, seed);
    extraction(arr_strings[62], &TotalLPSs_400AR, core, erlang, seed);
    extraction(arr_strings[63], &TotalLPSs_400IR, core, erlang, seed);
    extraction(arr_strings[64], &TotalLPSs_100, core, erlang, seed);
    extraction(arr_strings[65], &TotalLPSs_100AR, core, erlang, seed);
    extraction(arr_strings[66], &TotalLPSs_100IR, core, erlang, seed);
    extraction(arr_strings[67], &TotalLPSs_40, core, erlang, seed);
    extraction(arr_strings[68], &TotalLPSs_40AR, core, erlang, seed);
    extraction(arr_strings[69], &TotalLPSs_40IR, core, erlang, seed);
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
  avg_data(&maxTransponders);
  avg_data(&totalTransponders);
  avg_data(&CpR);
  avg_data(&avgHoldingTime);
  avg_data(&avgTransponders);
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
  avg_data(&totalBlocks_AR);
  avg_data(&totalBlocks_IR);
  avg_data(&totalBlocks_400);
  avg_data(&totalBlocks_400AR);
  avg_data(&totalBlocks_400IR);
  avg_data(&totalBlocks_100);
  avg_data(&totalBlocks_100AR);
  avg_data(&totalBlocks_100IR);
  avg_data(&totalBlocks_40);
  avg_data(&totalBlocks_40AR);
  avg_data(&totalBlocks_40IR);
  avg_data(&totalTransponders_AR);
  avg_data(&totalTransponders_IR);
  avg_data(&totalTransponders_400);
  avg_data(&totalTransponders_400AR);
  avg_data(&totalTransponders_400IR);
  avg_data(&totalTransponders_100);
  avg_data(&totalTransponders_100AR);
  avg_data(&totalTransponders_100IR);
  avg_data(&totalTransponders_40);
  avg_data(&totalTransponders_40AR);
  avg_data(&totalTransponders_40IR);
  avg_data(&avgTransponders_AR);
  avg_data(&avgTransponders_IR);
  avg_data(&avgTransponders_400);
  avg_data(&avgTransponders_400AR);
  avg_data(&avgTransponders_400IR);
  avg_data(&avgTransponders_100);
  avg_data(&avgTransponders_100AR);
  avg_data(&avgTransponders_100IR);
  avg_data(&avgTransponders_40);
  avg_data(&avgTransponders_40AR);
  avg_data(&avgTransponders_40IR);
  avg_data(&TotalLPSs_AR);
  avg_data(&TotalLPSs_IR);
  avg_data(&TotalLPSs_400);
  avg_data(&TotalLPSs_400AR);
  avg_data(&TotalLPSs_400IR);
  avg_data(&TotalLPSs_100);
  avg_data(&TotalLPSs_100AR);
  avg_data(&TotalLPSs_100IR);
  avg_data(&TotalLPSs_40);
  avg_data(&TotalLPSs_40AR);
  avg_data(&TotalLPSs_40IR);

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
  write_to_files(&maxTransponders);
  write_to_files(&totalTransponders);
  write_to_files(&CpR);
  write_to_files(&avgHoldingTime);
  write_to_files(&avgTransponders);
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
  write_to_files(&totalBlocks_AR);
  write_to_files(&totalBlocks_IR);
  write_to_files(&totalBlocks_400);
  write_to_files(&totalBlocks_400AR);
  write_to_files(&totalBlocks_400IR);
  write_to_files(&totalBlocks_100);
  write_to_files(&totalBlocks_100AR);
  write_to_files(&totalBlocks_100IR);
  write_to_files(&totalBlocks_40);
  write_to_files(&totalBlocks_40AR);
  write_to_files(&totalBlocks_40IR);
  write_to_files(&totalTransponders_AR);
  write_to_files(&totalTransponders_IR);
  write_to_files(&totalTransponders_400);
  write_to_files(&totalTransponders_400AR);
  write_to_files(&totalTransponders_400IR);
  write_to_files(&totalTransponders_100);
  write_to_files(&totalTransponders_100AR);
  write_to_files(&totalTransponders_100IR);
  write_to_files(&totalTransponders_40);
  write_to_files(&totalTransponders_40AR);
  write_to_files(&totalTransponders_40IR);
  write_to_files(&avgTransponders_AR);
  write_to_files(&avgTransponders_IR);
  write_to_files(&avgTransponders_400);
  write_to_files(&avgTransponders_400AR);
  write_to_files(&avgTransponders_400IR);
  write_to_files(&avgTransponders_100);
  write_to_files(&avgTransponders_100AR);
  write_to_files(&avgTransponders_100IR);
  write_to_files(&avgTransponders_40);
  write_to_files(&avgTransponders_40AR);
  write_to_files(&avgTransponders_40IR);
  write_to_files(&TotalLPSs_AR);
  write_to_files(&TotalLPSs_IR);
  write_to_files(&TotalLPSs_400);
  write_to_files(&TotalLPSs_400AR);
  write_to_files(&TotalLPSs_400IR);
  write_to_files(&TotalLPSs_100);
  write_to_files(&TotalLPSs_100AR);
  write_to_files(&TotalLPSs_100IR);
  write_to_files(&TotalLPSs_40);
  write_to_files(&TotalLPSs_40AR);
  write_to_files(&TotalLPSs_40IR);
}
