import requests
import re


def search(word):
        r = requests.get('https://www.google.com.hk/search?q=\"'+word+'\"')
        if r.status_code ==200:
                data = r.text
                s = data.find("\"resultStats\"")
                ss = data[s:s+100].find("</div>")
        else:
                print('bing')
                r = requests.get('https://www.bing.com/search?q=\"'+word+'\"')
                data = r.text
                s = data.find("\"sb_count\">")
                ss = data[s:s+40].find("</span>")
                if r.status_code !=200:
                        print('unresolved')
			return -1

        ans = re.findall(r'\d+',data[s:s+ss])
        num = 0;
        for i in range(0,len(ans)):
                num = num*1000+int(ans[i])
      
      
	print word,num
        return num

#open the query file
with open("queries.txt") as q:
        content = q.readlines()

out = open("output.txt","wb")

first = 0
second =0
i1=0
i2=0
for i,val in enumerate(content):
        print i,val
        counts =search(val)

        if(i%2==0):
                first = counts
        else:
                second = counts
         	if(first>second):
                        i1=1
                        i2=0
			out.write("1\n")
                else:
			if(first==second):
				out.write("0\n")
			else:
                        	i2=1
                        	i1=0
				out.write("2\n")
               
q.close()
out.close()            


