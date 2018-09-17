#coding=gbk
from xpinyin import Pinyin
import os

def is_chinese(uchar):
    if uchar >= u'\u4e00' and uchar <= u'\u9fa5' :
        return True
    else:
        return False

def is_number(uchar):
    if uchar >=u'\u0030' and uchar <= u'\u0039' :
        return True
    else:
        return False

def is_alphabet(uchar):
    if (uchar >= u'\0041' and uchar <= u'\005a') or (uchar >= u'\u0061' and uchar<=u'\u007a'):
        return True
    else:
        return False

class url_quality(object):
    __slots__=["url","quality"]

    def __init__(self,url,quality):
        self.url=url
        self.quality=quality

    def __lt__(self,temp):
        return self.quality>temp.quality

    def __le__(self,temp):
        return self.quality>=temp.quality

class keyword_url(object):
    __slots__=["keyword","url"]

    def __init__(self,keyword,url):
        self.keyword=keyword
        self.url=url



def url_searchtree_build(url,key_word_list):
    
    for i in key_word_list:
        flag=0
        
        for j in i:
            if is_chinese(j):
                flag=1
                #print("汉字")
                #print(flag)
                break
            elif is_alphabet(j):
                flag=2
                #print("字母")
                #print(flag)
                break
            elif is_number(j):
                flag=3
                #print("数字")
                #print(flag)
                break
        if flag==1:
            transform=Pinyin()
            #print(transform.get_pinyin(i[0]))

            first_pinyin=transform.get_pinyin(i[0])

            temp_path=''
            total_path=''

            for k in first_pinyin:
                
                temp_flag=0
                if temp_path=='':
                    temp_flag=1
                else:
                    temp_flag=0
                
                temp_path=temp_path+k

                if temp_flag==1:
                    total_path=total_path+temp_path+"/"
                else:
                    total_path=total_path+temp_path+'/'
                
                #print(total_path)
                if not os.path.isdir(total_path):
                    os.mkdir(total_path)
            #--------------------------------------------以上生成对应拼音树路径
                    

            #print(total_path)

            final_path=total_path+i[0]+'.txt'
    
            file_point = open(final_path,"a")
            for ii in key_word_list:
                temp_input=ii+' '
                file_point.write(temp_input)

            file_point.write("\n")
            file_point.write(url)
            file_point.write("\n")

            file_point.close()

            #---------------------------------------------以上更新对应的汉字文档并写入

        if flag==2:
            
            temp_path=''
            total_path=''          
            for k in i:
                
                temp_flag=0
                if temp_path=='':
                    temp_flag=1
                else:
                    temp_flag=0
                
                temp_path=temp_path+k

                if temp_flag==1:
                    total_path=total_path+temp_path+"/"
                else:
                    total_path=total_path+temp_path+'/'
                    
                #print(total_path)

                
                if not os.path.isdir(total_path):
                    os.mkdir(total_path)

            #-------------------------------------------以上生成字母检索树路径

            #print(total_path)

            final_path=total_path+temp_path+'.txt'
    
            file_point = open(final_path,"a")
            for ii in key_word_list:
                temp_input=ii+' '
                file_point.write(temp_input)

            file_point.write("\n")
            file_point.write(url)
            file_point.write("\n")

            file_point.close()

            #-------------------------------------------更新单词文档

        if flag==3:

            temp_path=''
            total_path=''          
            for k in i:
                
                temp_flag=0
                if temp_path=='':
                    temp_flag=1
                else:
                    temp_flag=0
                
                temp_path=temp_path+k

                if temp_flag==1:
                    total_path=total_path+temp_path+'/'
                else:
                    total_path=total_path+temp_path+'/'
                    
                #print(total_path)
                if not os.path.isdir(total_path):
                    os.mkdir(total_path)

            #---------------------------------------------以上产生数字检索树路径

            #print(total_path)

            final_path=total_path+temp_path+'.txt'
    
            file_point = open(final_path,"a")
            for ii in key_word_list:
                temp_input=ii+' '
                file_point.write(temp_input)

            file_point.write("\n")
            file_point.write(url)
            file_point.write("\n")

            file_point.close()

            #--------------------------------------------以上更新数字文档

            
#----------------------------------------------------------以上是URL检索树建立过程      
def url_searchtree_query(key_word_list):
    
    result_file_point=open("result_file.txt","w") 


    for i in key_word_list:
        flag=0

        #print(i)
        #print("---------------------------------------------")
        for j in i:
            if is_chinese(j):
                flag=1
                break
            elif is_alphabet(j):
                flag=2
                break
            elif is_number(j):
                flag=3
                break

        if flag==1:
            transform=Pinyin()
            #print(transform.get_pinyin(i[0]))

            first_pinyin=transform.get_pinyin(i[0])

            temp_path=''
            total_path=''

            for k in first_pinyin:
                
                temp_flag=0
                if temp_path=='':
                    temp_flag=1
                else:
                    temp_flag=0
                
                temp_path=temp_path+k

                if temp_flag==1:
                    total_path=total_path+temp_path
                else:
                    total_path=total_path+'/'+temp_path
                
                #print(total_path)

            #--------------------------------------------以上生成对应拼音树路径
                    

            #print(total_path)

            final_path=total_path+'/'+i[0]+'.txt'

            if os.path.isfile(final_path):
                file_point = open(final_path,"r")
                turn_exchange=0

                keyword_list=[]
                url_part=[]
                for line in open(final_path,"r"):

                    if turn_exchange==0:
                        keyword_list=line.split()
                        #print(keyword_list)
                        turn_exchange=1
                    else:
                        url_part=line.split()
                        #print(url_part)
                        turn_exchange=0

                        i_exist=0        #这个变量是当前的i检索关键词是否存在于文档中URL的关键词中
                        quality=0        #这个变量是检索关键词与文档URL关键词的匹配数量（决定这个URL的显示是靠前还是靠后）

                        for search_word in keyword_list:
                            if search_word==i:
                                i_exist=1

                        if i_exist==1:
                            for search_word in keyword_list:   #遍历文档中URL的关键词
                                for ii in key_word_list:       #遍历检索关键词
                                    if search_word==ii:
                                        quality=quality+1

                        #print(i_exist)
                        #print(quality)

                        if i_exist==1:
                            result_url_str=''.join(url_part)
                            #print(result_url_str)
                            
                            result_url_str=result_url_str+'\n'
                            result_file_point.write(result_url_str)
                            temp_quality_str=''+str(quality)+'\n'
                            result_file_point.write(temp_quality_str)

                file_point.close()
            else:
                print("no path")
                
        if flag==2:
            temp_path=''
            total_path=''          
            for k in i:
                
                temp_flag=0
                if temp_path=='':
                    temp_flag=1
                else:
                    temp_flag=0
                
                temp_path=temp_path+k

                if temp_flag==1:
                    total_path=total_path+temp_path
                else:
                    total_path=total_path+'/'+temp_path
                    
            #print(total_path)

            final_path=total_path+'/'+temp_path+'.txt'
            
            if os.path.isfile(final_path):
                file_point = open(final_path,"r")
                turn_exchange=0

                keyword_list=[]
                url_part=[]

                for line in open(final_path,"r"):

                    if turn_exchange==0:
                        keyword_list=line.split()
                        #print(keyword_list)
                        turn_exchange=1
                    else:
                        url_part=line.split()
                        #print(url_part)
                        turn_exchange=0

                        i_exist=0        #这个变量是当前的i检索关键词是否存在于文档中URL的关键词中
                        quality=0        #这个变量是检索关键词与文档URL关键词的匹配数量（决定这个URL的显示是靠前还是靠后）

                        for search_word in keyword_list:
                            if search_word==i:
                                i_exist=1

                        if i_exist==1:
                            for search_word in keyword_list:   #遍历文档中URL的关键词
                                for ii in key_word_list:       #遍历检索关键词
                                    if search_word==ii:
                                        quality=quality+1

                        #print(i_exist)
                        #print(quality)

                        if i_exist==1:
                            result_url_str=''.join(url_part)
                            #print(result_url_str)
                            
                            result_url_str=result_url_str+'\n'
                            result_file_point.write(result_url_str)
                            temp_quality_str=''+str(quality)+'\n'
                            result_file_point.write(temp_quality_str)

                
                file_point.close()
            else:
                print("no path")

        if flag==3:
            temp_path=''
            total_path=''          
            for k in i:
                
                temp_flag=0
                if temp_path=='':
                    temp_flag=1
                else:
                    temp_flag=0
                
                temp_path=temp_path+k

                if temp_flag==1:
                    total_path=total_path+temp_path
                else:
                    total_path=total_path+'/'+temp_path
                    
            #print(total_path)

            final_path=total_path+'/'+temp_path+'.txt'
            
            if os.path.isfile(final_path):
                file_point = open(final_path,"r")
                turn_exchange=0

                keyword_list=[]
                url_part=[]

                for line in open(final_path,"r"):

                    if turn_exchange==0:
                        keyword_list=line.split()
                        #print(keyword_list)
                        turn_exchange=1
                    else:
                        url_part=line.split()
                        #print(url_part)
                        turn_exchange=0

                        i_exist=0        #这个变量是当前的i检索关键词是否存在于文档中URL的关键词中
                        quality=0        #这个变量是检索关键词与文档URL关键词的匹配数量（决定这个URL的显示是靠前还是靠后）

                        for search_word in keyword_list:
                            if search_word==i:
                                i_exist=1

                        if i_exist==1:
                            for search_word in keyword_list:   #遍历文档中URL的关键词
                                for ii in key_word_list:       #遍历检索关键词
                                    if search_word==ii:
                                        quality=quality+1

                        #print(i_exist)
                        #print(quality)

                        if i_exist==1:    #如果关键词存在的话要把这个URL以及它的质量数写入result_file.txt文档中
                            result_url_str=''.join(url_part)
                            #print(result_url_str)
                            
                            result_url_str=result_url_str+'\n'
                            result_file_point.write(result_url_str)
                            temp_quality_str=''+str(quality)+'\n'
                            result_file_point.write(temp_quality_str)

                
                file_point.close()
            else:
                print("no path")
    
    result_file_point.close()    #到此为止 result_file.txt 文件中已经存储了若干URL和质量等级
    result_file_point=open("result_file.txt","r")   #现在要对重复的URL去重合并

    turn_exchange=0    #这个变量用来控制交替读入，一行URL，一行quality

    url_part=[]
    quality_part=[]

    main_operation_list=[]
    main_index=0;
    
    for line in open("result_file.txt","r"):
        if turn_exchange==0:
            url_part=line.split()   #将str转化成list
            #print(url_part)
            turn_exchange=1
        else:
            quality_part=line.split()
            #print(quality_part)
            turn_exchange=0

            result_url_str=''.join(url_part)    #将list转换为str
            result_quality_str=''.join(quality_part)    #将list转换为str

            temp_url_quality=url_quality(result_url_str,int(result_quality_str))  #构造新的url_quality类对象
            
            main_operation_list.append(temp_url_quality)    #直接使用list的append追加url_quality类的新对象

            main_index=main_index+1    #url_quality类对象的数量
            
    result_file_point.close()

    #print(main_index)

    #delete_index_list=[]
    
    for i in range(0,main_index):
        if i+1<main_index:

            for j in range(i+1,main_index):    #两次循环查重、更新最高的quality，要删除的item的quality会被标记为-1
                if main_operation_list[i].url==main_operation_list[j].url:
                    if main_operation_list[i].quality==main_operation_list[j].quality:
                        main_operation_list[j].quality=-1
                    elif main_operation_list[i].quality>main_operation_list[j].quality:
                        main_operation_list[j].quality=-1
                    elif main_operation_list[i].quality<main_operation_list[j].quality:
                        main_operation_list[i].quality=main_operation_list[j].quality
                        main_operation_list[j].quality=-1

    
    main_operation_list.sort()    #对结果list进行sort，被标记为重复的URL会被排序到list尾部

    #for item in main_operation_list:
        #print(item.url)
        #print(item.quality)

    if main_operation_list:    #检测“如果没有匹配网页”的话就不进行尾部删除操作，不加的话空的list会指针越界
        while main_operation_list[-1].quality==-1:    #将位于尾部的重复的URL进行删除
            del main_operation_list[-1]

    #print("==========================================")

    result_file_point=open("result_file.txt","w")   #将最后的结果再次写入result_file文件中
    for item in main_operation_list:
        print(item.url)
        print(item.quality)
        
        result_file_point.write(item.url)
        result_file_point.write("\n")
        result_file_point.write(str(item.quality))
        result_file_point.write("\n")

    result_file_point.close()

#------------------------------------------------------以上是URL检索树的查询过程
            
            
            
#------------------------------------------------------

test_list=["华南理工大学","scut","2016"]
#url_searchtree_build("www.scut.edu.cn",test_list)

#------------------------------------------------------以上是对新URL的添加
#------------------------------------------------------

test_list2=["华南理工大学","scut","2016","消化"]
#url_searchtree_query(test_list2)

#------------------------------------------------------以上是用关键词检索URL树获得的结果


def multi_url_import_from_txt():

    temp_keywords_list_point=open("temp_keywords_list.txt","a")

    turn_exchange=0

    url_part=[]
    keywords_part=[]
    
    for line in open("url_import.txt","r"):
        if turn_exchange==0:
            url_part=line.split()
            turn_exchange=1

        else:
            keywords_part=line.split()       
            turn_exchange=0

            #print("======================")
            #print(url_part)
            #print("----------------------")
            #print(keywords_part)
            #print("======================")

            result_url_str=''.join(url_part)    #将list转换为str

            url_searchtree_build(result_url_str,keywords_part)

            for item_keyword in keywords_part:
                temp_keywords_list_point.write(item_keyword)
                temp_keywords_list_point.write("\n")

    temp_keywords_list_point.close()
    print("妈的还有谁！！")

#-----------------------------------------------------------------以上是利用txt文件导入爬虫获得的网页            
    
#multi_url_import_from_txt()

#------------------------------------------------------------------



#------------------------------------------------------------------以下是夜间对每个节点txt内部做动态维护

def txt_maintain_in_night():

    keywords_part=[]
    keywords_str=''
    
    for line in open("temp_keywords_list.txt","r"):
        keywords_part=line.split()
        keywords_str=''.join(keywords_part)

        print(keywords_str)

        

        flag=0
        for i in keywords_str:
            if is_chinese(i):
                flag=1
                #print("汉字")
                #print(flag)
                break
            elif is_alphabet(i):
                flag=2
                #print("字母")
                #print(flag)
                break
            elif is_number(i):
                flag=3
                #print("数字")
                #print(flag)
                break
    
        if flag==1:
            transform=Pinyin()
            #print(transform.get_pinyin(i[0]))

            first_pinyin=transform.get_pinyin(keywords_str[0])

            temp_path=''
            total_path=''

            for k in first_pinyin:
                
                temp_flag=0
                if temp_path=='':
                    temp_flag=1
                else:
                    temp_flag=0
                
                temp_path=temp_path+k

                if temp_flag==1:
                    total_path=total_path+temp_path+"/"
                else:
                    total_path=total_path+temp_path+'/'
                
                
                if not os.path.isdir(total_path):
                    os.mkdir(total_path)
        #--------------------------------------------------------以上生成对应拼音树路径
                    

            #print(total_path)

            main_operation_list=[]
            count=0

            final_path=total_path+keywords_str[0]+'.txt'

            if os.path.isfile(final_path):
                turn_exchange=0

                keyword_list=[]
                url_part=[]

                for line in open(final_path,"r"):
                    if turn_exchange==0:
                        keyword_list=line.split()      
                        turn_exchange=1

                    else:
                        url_part=line.split()                
                        turn_exchange=0
                        result_url_str=''.join(url_part)
                        
                        temp_keyword_url=keyword_url(keyword_list,result_url_str)  #构造新的url_quality类对象
            
                        main_operation_list.append(temp_keyword_url)

                        count=count+1

            for item in main_operation_list:
                print("===================")
                print(item.keyword)
                print(item.url)
                print("====================")

            print(count)

            if count>=300:
                diff=count-300

                for j in range(0,diff):
                    del main_operation_list[0]

                url_txt_point=open(final_path,"w")

                for item in main_operation_list:
                    for key in item.keyword:
                        temp_input=key+' '
                        url_txt_point.write(temp_input)

                    url_txt_point.write("\n")
                    url_txt_point.write(item.url)
                    url_txt_point.write("\n")

                url_txt_point.close()

        if flag==2:
            
            temp_path=''
            total_path=''          
            for k in keywords_str:
                
                temp_flag=0
                if temp_path=='':
                    temp_flag=1
                else:
                    temp_flag=0
                
                temp_path=temp_path+k

                if temp_flag==1:
                    total_path=total_path+temp_path+"/"
                else:
                    total_path=total_path+temp_path+'/'
                    
                #print(total_path)

                
                if not os.path.isdir(total_path):
                    os.mkdir(total_path)

            #-------------------------------------------以上生成字母检索树路径

            main_operation_list=[]
            count=0

            #print(total_path)

            final_path=total_path+temp_path+'.txt'

            if os.path.isfile(final_path):
                turn_exchange=0

                keyword_list=[]
                url_part=[]

                for line in open(final_path,"r"):
                    if turn_exchange==0:
                        keyword_list=line.split()      
                        turn_exchange=1

                    else:
                        url_part=line.split()                
                        turn_exchange=0
                        result_url_str=''.join(url_part)
                        
                        temp_keyword_url=keyword_url(keyword_list,result_url_str)  #构造新的url_quality类对象
            
                        main_operation_list.append(temp_keyword_url)

                        count=count+1

            for item in main_operation_list:
                print("===================")
                print(item.keyword)
                print(item.url)
                print("====================")

            print(count)

            if count>=300:
                diff=count-300

                for j in range(0,diff):
                    del main_operation_list[0]

                url_txt_point=open(final_path,"w")

                for item in main_operation_list:
                    for key in item.keyword:
                        temp_input=key+' '
                        url_txt_point.write(temp_input)

                    url_txt_point.write("\n")
                    url_txt_point.write(item.url)
                    url_txt_point.write("\n")

                url_txt_point.close()

        if flag==3:
            temp_path=''
            total_path=''          
            for k in keywords_str:
                
                temp_flag=0
                if temp_path=='':
                    temp_flag=1
                else:
                    temp_flag=0
                
                temp_path=temp_path+k

                if temp_flag==1:
                    total_path=total_path+temp_path+"/"
                else:
                    total_path=total_path+temp_path+'/'
                    
                #print(total_path)

                
                if not os.path.isdir(total_path):
                    os.mkdir(total_path)

            #-------------------------------------------以上生成字母检索树路径

            main_operation_list=[]
            count=0

            #print(total_path)

            final_path=total_path+temp_path+'.txt'

            if os.path.isfile(final_path):
                turn_exchange=0

                keyword_list=[]
                url_part=[]

                for line in open(final_path,"r"):
                    if turn_exchange==0:
                        keyword_list=line.split()      
                        turn_exchange=1

                    else:
                        url_part=line.split()                
                        turn_exchange=0
                        result_url_str=''.join(url_part)
                        
                        temp_keyword_url=keyword_url(keyword_list,result_url_str)  #构造新的url_quality类对象
            
                        main_operation_list.append(temp_keyword_url)

                        count=count+1

            for item in main_operation_list:
                print("===================")
                print(item.keyword)
                print(item.url)
                print("====================")

            print(count)

            if count>=300:
                diff=count-300

                for j in range(0,diff):
                    del main_operation_list[0]

                url_txt_point=open(final_path,"w")

                for item in main_operation_list:
                    for key in item.keyword:
                        temp_input=key+' '
                        url_txt_point.write(temp_input)

                    url_txt_point.write("\n")
                    url_txt_point.write(item.url)
                    url_txt_point.write("\n")

                url_txt_point.close()



    #keyword_list_file_point=open("temp_keywords_list.txt","w")  #将该文件进行清空
    #keyword_list_file_point.close()

#===================================================================

#txt_maintain_in_night()


   
