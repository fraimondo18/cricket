import scipy.io
import sys

import numpy as np

from ApproximateEntropy import ApproximateEntropy

import scipy.stats as st

import csv
import os


def separateLogs(fName):
    fileId1 = open('fileID1.txt', 'w')
    fileId2 = open('fileID2.txt', 'w')

    with open(fName) as file:
        for line in file:
            
            idIdx = line.rfind('ID:1')
            if idIdx != -1:
                fileId1.write(line[idIdx+4:].replace('\t', ''))
            else:
                idIdx = line.rfind('ID:2')
                fileId2.write(line[idIdx+4:].replace('\t', ''))              
                
                
def main():
    
    chSeqList = list()
    experimentList = list()
    chMismatchList = list()
    totalBlockSize = 0
    invBlockSize = 0
    numBlkSize = 0
    chSeqBigStr = ''
    chRssiBigStr = ''
    minInvBlkSize = 1000
    maxInvBlkSize = 0
    max_n = 0
    
    
    whichDevice = 'Alice'
    csvFileName = 'test'+ whichDevice + '.csv'
    meanCsvfile = 'meanValues'+ whichDevice + '.csv'
    fileList = ['kgA_0411.txt','kgA_0711.txt','kgA_0811_p2.txt','kgA_0811.txt','kgA_0911_day.txt','kgA_0911.txt','kgA_1011.txt']
    #fileList = ['kgB_0411.txt','kgB_0711.txt','kgB_0811.txt','kgB_0911_day.txt','kgB_0911.txt']
  
    
    # Only for Cooja log files
    #for fName in fileList:
    #    separateLogs(fName)
    #fileList = ['fileID1.txt']
    
                
    expCount = 0
    for fName in fileList:
        with open(fName) as file:
            for line in file:
                
                if line.find('kg_rssi:') != -1:
                    chRssiBigStr = chRssiBigStr + (next(file).rstrip().replace(' ',''))
                    
                if line.find('Channel sequence') != -1:
                    chSeq = next(file).rstrip().replace(' ','')
                    chSeqBigStr = chSeqBigStr + chSeq
                    chSeqList.append(chSeq)
                
                word = 'mismatch:'
                if line.find(word) != -1:
                    wordEndIndex = line.index(word) + len(word)
                    chMismatchList.append(int(line[wordEndIndex:line.find('%')]))   
                #Mispelling in the old logs
                word = 'missmatch:'
                if line.find(word) != -1:
                    wordEndIndex = line.index(word) + len(word)
                    chMismatchList.append(int(line[wordEndIndex:line.find('%')])) 
                                             
                word1 = 'kg_block_size'
                word2 = 'encode ini_key_len'
                word3 = 'decode ini_key_len'
                if line.find(word1) != -1 and (line.find(word2) != -1 or line.find(word3) != -1):
                    wordEndIndex = line.index(word1) + len(word1)
                    bSize = int(line[wordEndIndex:])
                   
                    if((1/bSize)<minInvBlkSize):
                        minInvBlkSize = (1/bSize)
                    
                    if((1/bSize)>maxInvBlkSize):
                        maxInvBlkSize = (1/bSize)
                            
                    totalBlockSize = totalBlockSize + bSize
                    if invBlockSize == 0:
                        invBlockSize = 1/bSize
                    else:
                        invBlockSize = invBlockSize + 1/bSize
                    numBlkSize = numBlkSize + 1
                    
                    if(bSize > max_n):
                        max_n = bSize
                    
                if line.find('KEY ready') != -1:
                    experimentInfo = {}
                    expCount = expCount + 1
                    experimentInfo['index'] = expCount
                    experimentInfo['meanBlockSize'] = totalBlockSize/numBlkSize
                    experimentInfo['meanInvBlockSize'] = invBlockSize/numBlkSize
                    experimentInfo['max_n'] = max_n
                    experimentInfo['chSeqList'] = chSeqList
                    experimentInfo['rssi'] = chRssiBigStr
                    finalKey = next(file).rstrip()
                    experimentInfo['finalKey'] = finalKey.replace(' ','')
                    ape, passed = ApproximateEntropy.approximate_entropy_test(chSeqBigStr)
                    experimentInfo['ape'] = ape
                    experimentInfo['mininvBlkSize'] = minInvBlkSize
                    experimentInfo['maxinvBlkSize'] = maxInvBlkSize
                    experimentInfo['chMismatch'] = np.mean(chMismatchList)
                    
                    chMismatchList = list()
                    chSeqBigStr = ''
                    chRssiBigStr = ''
                    minInvBlkSize = 1000
                    maxInvBlkSize = 0
                    totalBlockSize = 0
                    numBlkSize = 0
                    invBlockSize = 0
                    max_n = 0
                
                word = 'mismatches'
                if line.find(word) != -1:
                    wordEndIndex = line.index(word) + len(word)
                    experimentInfo['mismatches'] = int(line[wordEndIndex:])
                
                word = 'KDR (*1000)'
                if line.find(word) != -1:
                    wordEndIndex = line.index(word) + len(word)
                    experimentInfo['KDR'] = float(line[wordEndIndex:])/1000
                
                word = 'Encoding rate (*1000)'
                if line.find(word) != -1:
                    wordEndIndex = line.index(word) + len(word)
                    experimentInfo['encodingRate'] = float(line[wordEndIndex:])/1000
                 
                word = 'total_num_ch_sequences'
                if line.find(word) != -1:
                    wordEndIndex = line.index(word) + len(word)
                    experimentInfo['num_ch_sequences'] = int(line[wordEndIndex:])
                    experimentList.append(experimentInfo)
                    chSeqList = list()
       
    
       
    # Removelow entropy experiments 
    expCount=0
    experimentListFiltered = list()
    for expItem in experimentList:
        for k, v in expItem.items():
            if k == 'ape':
                if v > 0.3:
                    expCount = expCount + 1
                    expItem['index'] = expCount
                    experimentListFiltered.append(expItem)
                    break
                
                
    expCount=0
    blkSizeList = list()
    invBlkSizeList = list() 
    kdrList = list()
    encRateList = list()  
    chMismatchList = list() 
    apeList = list() 
    minInvNList = list()
    maxInvNList = list()
    numChSeqList = list()
    csvHeader = list()
    writeHeader = 1
    ape = 0
    try:
        with open(csvFileName, mode='w') as data_file:
            data_writer = csv.writer(data_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)     
            for expItem in experimentListFiltered:
                expCount = expCount + 1
                dataItem = list()
                for k, v in expItem.items():
                    if k == 'index':
                        csvHeader.append('Exp')
                        dataItem.append(v)
                    if k == 'mismatches':
                        csvHeader.append('dist(kA-kB)')
                        dataItem.append(int(v))
                    if k == 'KDR':
                        csvHeader.append('KDR')
                        kdrList.append(v)
                        dataItem.append("%.4f" % v)
                    if k == 'mininvBlkSize':
                        csvHeader.append('min(1/n)')
                        dataItem.append("%.3f" % v)
                        minInvNList.append(v)
                    if k == 'maxinvBlkSize':
                        csvHeader.append('max(1/n)')
                        dataItem.append("%.3f" % v)
                        maxInvNList.append(v)
                    if k == 'encodingRate':
                        csvHeader.append('R')
                        encRateList.append(v)
                        dataItem.append("%.4f" % v)
                    if k == 'meanBlockSize':
                        blkSizeList.append(v)
                        csvHeader.append('E[n]')
                        dataItem.append("%.3f" % v)
                    if k == 'ape':
                        ape = v
                        csvHeader.append('H')
                        apeList.append(v)
                        dataItem.append("%.3f" % v)
                    if k == 'chMismatch':
                        chMismatchList.append(v)
                        csvHeader.append('p_ch')
                        dataItem.append("%.3f" % v)
                    if k == 'num_ch_sequences':
                        csvHeader.append('messages')
                        dataItem.append(int(v/2) + 32)    #Assuming 2-bits quantization plus 32 RSSI samples to calculate mismatch
                        numChSeqList.append(int(v/2) + 32)
                    if k == 'meanInvBlockSize':
                        invBlkSizeList.append(v)
                        csvHeader.append('E[1/n]')
                        dataItem.append("%.3f" % v)
                    if k == 'max_n':
                        csvHeader.append('max(n)')
                        dataItem.append(v)
                if writeHeader == 1:
                    data_writer.writerow(csvHeader)
                    writeHeader = 0
                if ape > 0.3:
                    data_writer.writerow(dataItem)
                
    except OSError:
        print('IO file error, discarding sample')            
    

    cLev = 0.95 
    
    interval = st.t.interval(alpha=cLev, df=len(encRateList)-1, loc=np.mean(encRateList), scale=st.sem(encRateList))
    mEncRate = np.mean(encRateList)
    ciEncRate = (interval[1] - interval[0])/2    
    interval = st.t.interval(alpha=cLev, df=len(apeList)-1, loc=np.mean(apeList), scale=st.sem(apeList))
    mH = np.mean(apeList)
    ciH = (interval[1] - interval[0])/2    
    interval = st.t.interval(alpha=cLev, df=len(blkSizeList)-1, loc=np.mean(blkSizeList), scale=st.sem(blkSizeList))
    mEn = np.mean(blkSizeList)
    ciEn = (interval[1] - interval[0])/2    
    interval = st.t.interval(alpha=cLev, df=len(invBlkSizeList)-1, loc=np.mean(invBlkSizeList), scale=st.sem(invBlkSizeList))
    mEinvn = np.mean(invBlkSizeList)
    ciEinvn = (interval[1] - interval[0])/2    
    interval = st.t.interval(alpha=cLev, df=len(chMismatchList)-1, loc=np.mean(chMismatchList), scale=st.sem(chMismatchList))
    mChMism = np.mean(chMismatchList)
    ciChMism = (interval[1] - interval[0])/2    
    
   
    try:
        with open(meanCsvfile, mode='w') as data_file:
            data_writer = csv.writer(data_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)     
            dataItem = list()
            csvHeader = list()
            
            interval = st.t.interval(alpha=cLev, df=len(kdrList)-1, loc=np.mean(kdrList), scale=st.sem(kdrList))
            mKDR = np.mean(kdrList)
            ciKDR = (interval[1] - interval[0])/2

            csvHeader.append('KDR')
            dataItem.append("%.4f" % mKDR + '+/-' + "%.4f" % ciKDR)
            
            csvHeader.append('R')
            dataItem.append("%.3f" % mEncRate + '+/-' + "%.3f" % ciEncRate)
            
            csvHeader.append('H')
            dataItem.append("%.3f" % mH + '+/-' + "%.3f" % ciH)
            
            csvHeader.append('E[n]')
            dataItem.append("%.3f" % mEn + '+/-' + "%.3f" % ciEn)
            
            csvHeader.append('E[1/n]')
            dataItem.append("%.3f" % mEinvn + '+/-' + "%.3f" % ciEinvn)
            
            csvHeader.append('p_ch')
            dataItem.append("%.3f" % mChMism + '+/-' + "%.3f" % ciChMism)
            
            data_writer.writerow(csvHeader)
            data_writer.writerow(dataItem)
                
    except OSError:
        print('IO file error, discarding sample')   
        
    

if __name__ == '__main__':
    main()
