'''
  Team Id: HC#145
  Author List:  Sujan Bag
  Filename: task4.py
  Theme: Homecoming (HC) 
  Functions: findhabit(image),findanimal(image),Hpredict_image(image_path,model),Apredict_image(image_path,model),Diff(li1,li2)
  Global Variables: position=[],hposition=[],aposition=[],name=[],hname=[],aname=[],dicto={},animallist={},habitatlist={},Amodel,
  Aclass_name,Amodel1,Hmodel,Hclass_name,Hmodel1,hab,data,habitatandanimalllist,handa,flag,habit,animal,habitatloc,animalloc,dictokey,
  valid_habitat,invalid_habitat,fullstr,printstr,file,ser,file1,text,x,u,v,a,b,k,x,c,d,i,j,x,t,ap,df,animalmodelpath,habitmodelpath,excel_file_name,img
 '''

import serial
import datetime
import torch
from PIL import Image
from torch.autograd import Variable
import torchvision
from torchvision import datasets, models, transforms
import cv2
import argparse
import torch
import pandas as pd
import warnings
#ignore the warnning 
warnings.filterwarnings("ignore")


ap=argparse.ArgumentParser()
ap.add_argument("input",help="input a arena image") #input for taking arena image as a argument
ap.add_argument("-s","--save",help="save contoured image") #for saving "-s" argument
ap.add_argument("-amod","--animalmodel",help="path of animal model") #for providing animal model -amod and location 
ap.add_argument("-homd","--habitatmodel",help="path of habitat model")#for providing habitat model -hmod and location
ap.add_argument("-excel","--mappingfile",help="path of mapping file")#for animal and habitat mapping -excel take a excel file only
args=ap.parse_args()

if args.animalmodel != None:
    animalmodelpath=args.animalmodel
else:
    animalmodelpath="divide2PerfectAnimalModel.pth" #by default it's take animal model file from it's current directory

if args.mappingfile != None:
    excel_file_name=args.mappingfile
else:
    excel_file_name="Animal_Habitat_Mapping.xlsx" #by default it's take animal habitat mapping file location from it's current directory


if args.habitatmodel != None:
    habitatmodelpath=args.habitatmodel
else:
    habitatmodelpath='dividePerfectHabitatModel.pth'#by default it's take habitat model location from it's current working directory
img=args.input


df=pd.read_excel(excel_file_name)#read the mapping excel file



device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

position=[]
hposition=[]
aposition=[]
name=[]
hname=[]
aname=[]
dicto={}
animallist={}
habitatlist={}
image=cv2.imread(img)

Amodel=torch.load(animalmodelpath,map_location=device) #load animal model
Aclass_name=Amodel['class_name'] #copy all the class name of this model in Aclass_name variable
Amodel1=Amodel['arch']#copy entire model in Amodel1

Hmodel=torch.load(habitatmodelpath,map_location=device)#load habitat model
Hclass_name=Hmodel['class_name'] #copy All the class name of this model in Hclass_name variable
Hmodel1=Hmodel['arch'] #copy entire model in Hmodel1

'''
Function name    :  findhabit(image)
input            :  image
output           :  predicted class name
call example     :  a=findhabit(image)
'''
def findhabit(image):
    image=Image.fromarray(image,'RGB')
    index=Hpredict_image(image,Hmodel1)
    prediction=Hclass_name[index]
    return prediction
'''
Function name    :  findanimal(image)
input            :  image
output           :  predicted class name
call example     :  a=findanimal(image)
'''
def findanimal(image):
    image=Image.fromarray(image,'RGB')
    index=Apredict_image(image,Amodel1)
    prediction=Aclass_name[index]
    return prediction


'''
Function name    :  Hpredict_image(image_path,model)
input            :  image path and model 
output           :  predicted class name index of Habitat image
call example     :  a=Hpredict_image(image_path,model1)
'''

def Hpredict_image(image_path,model1):
    #print("Prediction in progress")
    image=image_path
    #image = Image.open(image_path,'rb')

    # Define transformations for the image, should (note that imagenet models are trained with image size 224)
    transformation = transforms.Compose([
        transforms.Resize(224),
        #transforms.CenterCrop(224),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])

    ])

    # Preprocess the image
    image_tensor = transformation(image).float()

    # Add an extra batch dimension since pytorch treats all images as batches
    image_tensor = image_tensor.unsqueeze_(0)

    if torch.cuda.is_available():
        image_tensor.cuda()

    # Turn the input into a Variable
    input = Variable(image_tensor)
    input=input.to(device)

    # Predict the class of the image
    output = model1(input)

    index = output.cpu().data.numpy().argmax()

    return index


'''
Function name    :  Apredict_image(image_path,model)
input            :  image path and model 
output           :  predicted class name index of Animal image
call example     :  a=Apredict_image(image_path,model1)
'''

#this function will predict image
def Apredict_image(image_path,model1):
    #print("Prediction in progress")
    #image = Image.open(image_path)
    image=image_path
    model_ft=model1

    # Define transformations for the image, should (note that imagenet models are trained with image size 224)
    '''transformation = transforms.Compose([
        transforms.Resize(input_size),
        transforms.ToTensor(),
        transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))

    ])'''
    transformation=transforms.Compose([
        transforms.Resize(224),
        transforms.CenterCrop(224),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ])

    # Preprocess the image
    image_tensor = transformation(image).float()

    # Add an extra batch dimension since pytorch treats all images as batches
    image_tensor = image_tensor.unsqueeze_(0)

    if torch.cuda.is_available():
        image_tensor.cuda()

    # Turn the input into a Variable
    input = Variable(image_tensor)
    input=input.to(device)

    # Predict the class of the image
    output = model_ft(input)

    index = output.cpu().data.numpy().argmax()

    return index


#x is a variable which will count number of contour image
#This will draw contour and predict all the habitat image 
x=1
for i in range(0,5):
    for j in range(0,5):
        image2=image[1629-i*310:1930-i*310,390+j*310:690+j*310,:] #habitat location of arena image 
        #cv2.imshow('image2',image2)
        #cv2.waitKey(0)
        #cv2.destroyAllWindows()
        imggray=cv2.cvtColor(image2,cv2.COLOR_BGR2GRAY)
        _,thres=cv2.threshold(imggray,220,225,0)
        _,contures,_=cv2.findContours(thres,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) #find conture of habitat image
       # print(len(contures))
        if len(contures) != 1:
            pred=findhabit(image[1639-i*310:1922-i*310,396+j*310:680+j*310,:])#predict class name of habitat image
           # print(x,pred)
            position.append(x)
            hposition.append(x)
            name.append(pred)
            hname.append(pred)
            dicto=dict(zip(position,name))
            habitatlist=dict(zip(hposition,hname))
            image[1629-i*310:1930-i*310,390+j*310:690+j*310,:]=cv2.drawContours(image2,contures,0,(0,255,0),4)
            val=x
            cv2.putText(image2,str(val),(80,150),cv2.FONT_HERSHEY_SIMPLEX,1.8,(0,0,255),2)
            #cv2.imshow('con',image)
            #cv2.waitKey(0)
            #cv2.destroyAllWindows()
        x=x+1

#top corner
u=0
v=0

for i in range(0,2):
    image3=image[120:265,120+u:264+v,:] #location of image
    image11=image[90:265,120+u:264+v,:]
    img10gray=cv2.cvtColor(image3,cv2.COLOR_BGR2GRAY)
    _,thres=cv2.threshold(img10gray,220,225,0)
    _,contures,_=cv2.findContours(thres,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)#find conture of image location
   # print(len(contures))
    if len(contures) !=3:
        pred=findanimal(image[120:265,120+u:264+v,:])#prediction of  animal image
        image[120:265,120+u:264+v,:]=cv2.drawContours(image3,contures,1,(0,255,0),2)
        if i==0:
            value='A6'
        else:
            value='F6'
        cv2.putText(image11,value,(50,30),cv2.FONT_HERSHEY_SIMPLEX,0.8,(0,0,0),2)
        #cv2.imshow('track',image)
        #cv2.imshow('im',image[120:265,120+u:264+v,:])
        #cv2.waitKey(0)
        #cv2.destroyAllWindows()
        #print(value,pred)
        position.append(value)
        aposition.append(value)
        name.append(pred)
        aname.append(pred)
        dicto=dict(zip(position,name))
        animalliston=dict(zip(aposition,aname))
    u=u+1936
    v=v+1937

#bottom two corner contour find ,drawing and prediction

u=0
v=0
for i in range(0,2):
    image7=image[2055:2200,120+u:265+v,:]#image location copy to image7
    image8=image[2025:2200,120+u:265+v,:]
    img7gray=cv2.cvtColor(image7,cv2.COLOR_BGR2GRAY)
    _,thres=cv2.threshold(img7gray,220,225,0)
    _,contures,_=cv2.findContours(thres,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)#find conture 
    #print(len(contures))
    if len(contures) != 3:
        pred=findanimal(image[2074:2181,138+u:249+v,:])#predict animal name 
        image[2055:2200,120+u:265+v,:]=cv2.drawContours(image7,contures,1,(0,255,0),2)
        if i==0:
            value='A1'
        else:
            value='F1'
        cv2.putText(image8,value,(50,30),cv2.FONT_HERSHEY_SIMPLEX,0.8,(0,0,0),2)
        #cv2.imshow('images',image)
        #cv2.imshow('track',image[2055:2200,120+u:265+v,:])
        #cv2.waitKey(0)
        #cv2.destroyAllWindows()
       # print(value,pred)
        position.append(value)
        aposition.append(value)
        name.append(pred)
        aname.append(pred)
        dicto=dict(zip(position,name))
        animalliston=dict(zip(aposition,aname))
    u=u+1936
    v=v+1937
    
 #top to bottom contour find drawing and detection
a=0
b=0
k=0
x=0
for j in range(0,4):
    c=0
    d=0
    for i in range(0,2):
        image3=image[2055-c:2200-d,622+a:766+b,:] #location of arena image
        image13=image[2025-c:2200-d,622+a:766+b,:]
        img7gray=cv2.cvtColor(image3,cv2.COLOR_BGR2GRAY)
        _,thres=cv2.threshold(img7gray,220,225,0)
        _,contures,_=cv2.findContours(thres,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)#find all conture
        #print(len(contures))
        pred=findanimal(image[2075-c:2182-d,636+a:753+b,:]) #predict animal name
        if len(contures) !=3:
            image[2055-c:2200-d,622+a:766+b,:]=cv2.drawContours(image3,contures,1,(0,255,0),2)
            if i==0:
                value=chr(ord('B')+x)+'1'
            else:
                value=chr(ord('B')+x)+'6'
            cv2.putText(image13,value,(50,30),cv2.FONT_HERSHEY_SIMPLEX,0.8,(0,0,0),2)
            #cv2.imshow('track',image)
            #cv2.imshow('image4',image[2055-c:2200-d,622+a:766+b,:])
            #cv2.waitKey(0)
            #cv2.destroyAllWindows()
            #print(value,pred)
            position.append(value)
            aposition.append(value)
            name.append(pred)
            aname.append(pred)
            dicto=dict(zip(position,name))
            animalliston=dict(zip(aposition,aname))
        c=c+1935
        d=d+1935
    x=x+1
    a=a+311
    b=b+309

#Two Side Left-Right contour detection drawing and prediction
a=0
b=0
k=0
for j in range(0,2):
    x=2
    for i in range(0,4):
        image1=image[1552-i*310:1697-i*310,120+a:265+b,:]#location of arena image 
        image14=image[1522-i*310:1697-i*310,120+a:265+b,:]
        img1gray=cv2.cvtColor(image1,cv2.COLOR_BGR2GRAY)
        _,thres=cv2.threshold(img1gray,220,225,0)
        _,contures,_=cv2.findContours(thres,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)#find conture of image location
        #print(len(contures))
        if len(contures) !=3:
            pred=findanimal(image[1569-i*309:1676-i*311,140+a:244+b,:]) #predict animal name 
            image[1552-i*310:1697-i*310,120+a:265+b,:]=cv2.drawContours(image1,contures,1,(0,255,0),2)
            if j==0:
                val='A'+str(x)
            else:
                val='F'+str(x)
            cv2.putText(image14,val,(50,30),cv2.FONT_HERSHEY_SIMPLEX,0.8,(0,0,0),2)
            #cv2.imshow('track',image[1552-i*310:1697-i*310,120+a:265+b,:])
            #cv2.imshow('ori',image)
            #cv2.waitKey(0)
            #cv2.destroyAllWindows()
            #print(val,pred)
            position.append(val)
            aposition.append(value)
            name.append(pred)
            aname.append(pred)     
            dicto=dict(zip(position,name))
            animalliston=dict(zip(aposition,aname))
            x=x+1
        else:
            x=x+1
    a=a+1933
    b=b+1936
    


print('\n Animal And Habitat : ')
print("__________________________")
print(dicto) #this will print animal and habitat name with location
'''for i in dicto.keys():
    print(dicto[i])'''
'''print('\nHabitat(Cell Numbers)')
print(habitatlist)'''
print("For Animal Dataset")
print("..................")
print('\nAnimal(Location)')
print('__________________\n')
print(animalliston)
a,b=df.shape #assign excel sheet column and row size in a and b variable
hab=[]

for i in range(0,a):
    hab.append(df.iloc[i][0])#copy all habitat name of excell file in hab list
    

    
data={}


for i in range(0,a):
    for j in range(0,b):
        data.update({hab[i]:df.iloc[i][0:]}) 
        
        
#all the habitat and animal which are maching to excel file copy to habitatandanimal list        
habitatandanimallist=[]
for x in hab:
    for y in dicto.keys():
        if(x==dicto[y]):
            listOfhabitat = [key  for (key, value) in dicto.items() if value == x]
           # print(x,listOfhabitat)
            habitatandanimallist.append(listOfhabitat)
            for z in range(1,b):
                for t in dicto.keys():
                    if(data[x][z]==dicto[t]):
                        #habitatandanimallist.append('\n')
                        listofanimal= [key  for (key, value) in dicto.items() if value == data[x][z]]
                       # print(data[x][z],listofanimal)
                        #habitatandanimallist.append('\n')
                        habitatandanimallist.append(listofanimal)
                        #habitatandanimallist.append('\n') 
                        break
            #habitatandanimallist.append('\n')
                                   
            break
 

handa=[]
flag=0
i=0
while(i<len(habitatandanimallist)):
    j=i+1
    while(j<len(habitatandanimallist)):
        if(habitatandanimallist[i]==habitatandanimallist[j]):
            print(habitatandanimallist[i],i)
            flag=1
            i=i+1
        else:
            flag=0
        j=j+1
    if(flag==0):
        handa.append(habitatandanimallist[i])
            
    i=i+1



habitatandanimallist=handa          

#separate habitat and animal             
i=0
habit=[]
animal=[]
while(i <len(habitatandanimallist)):
    if(type(habitatandanimallist[i][0])==str):
        habit.append(habitatandanimallist[i-1])
        animal.append(habitatandanimallist[i])
        #while j in range(i+1,len(habitatandanimallist)):
        j=i+1
        while(j<len(habitatandanimallist)):
            if(type(habitatandanimallist[j][0])==str):
                animal.append(habitatandanimallist[j])
                habit.append(habitatandanimallist[i-1])
                i=i+1
                j=j+1
            else:
                break
                
    i=i+1
    
    
 #according to mapping rearrange habitat and animal  
i=0
habitatloc=[]
animalloc=[]
while(i<len(animal)):
    if(len(animal[i])==len(habit[i])):
        l=0
        while(l<len(habit[i])):
            habitatloc.append(habit[i][l])
            l=l+1
        #print('animal=habit')
        i=i+1
    elif(len(animal[i])>len(habit[i])):
        j=0
       # print('animal greater')
        while(j<len(habit[i])):
            habitatloc.append(habit[i][j])
            j=j+1
        k=0
        while(k<(len(animal[i])-len(habit[i]))):
            habitatloc.append(habit[i][0])
            k=k+1
              
        i=i+1
    else:
        j=0
        while(j<len(animal[i])):
            habitatloc.append(habit[i][j])
            j=j+1
        i=i+1
t=0
while(t<len(animal)):
    for j in range(0,len(animal[t])):
        animalloc.append(animal[t][j])
    t=t+1




dictokey=[]
for key in habitatlist:
    dictokey.append(key)
def Diff(li1, li2): 
    return (list(set(li1) - set(li2))) 
habitat_loc=Diff(dictokey,habitatloc)

invalid_habitat=[]
for i in range(0,len(habitat_loc)):
    invalid_habitat.append([habitat_loc[i],habitatlist[habitat_loc[i]]])

valid_habitat=[]
for i in range(0,len(habitatloc)):
    valid_habitat.append([habitatloc[i],habitatlist[habitatloc[i]]])
print("For Habitat Dataset")
print("....................")
print("\nValid habitat set :")
print("___________________\n")
print(valid_habitat)

print("\nInvalid habitat set :")
print("______________________\n")
print(invalid_habitat)

#Only two animal are associated with one habitat acording to Theme Rule
animal=[]
habitat=[]
i=0
while(i<len(habitatloc)):
    animal.append(animalloc[i])
    habitat.append(habitatloc[i])
    j=i+1
    count=1
    while(j<len(habitatloc)):
        if(habitatloc[i]==habitatloc[j]):
            count=count+1
        j=j+1
    if(count>2):
        print(dicto[animalloc[i]])
           
        i=i+1
        
    i=i+1    
fullstr=(str(habitat)+'\n'+str(animal))#all animal and habitat convert to string and store it in fullstr variable
printstr=('Animals = '+str(animal)+'\n'+'Habitats = '+str(habitat)) #This string will print in output screen

fullstr=fullstr.replace("'",'')#remove ' 
fullstr=fullstr.replace("[",'')#remove [
fullstr=fullstr.replace("]",'')#remove ]

printstr=printstr.replace("'",'')#remove ' 
'''printstr=printstr.replace("[",'')#remove [
printstr=printstr.replace("]",'')#remove ]
'''
#create a text file for this fullstr text file

file=open("textfileofanimalandhabitat.txt","w") 
file.writelines(fullstr)
file.close()
print('\n After Mapping of animal and habitat this is only locations of animal and habitat :')
print("_______________________________________________________________________________________\n")
print(printstr)
#if save argument passed then it will save the drawing contour image
if args.save != None:
    cv2.imwrite(args.save,image)
    print('successful to save ......')
ser=serial.Serial()
ser.port="com3"
ser.baudrate=9600
print(ser.portstr)
file1=open("textfileofanimalandhabitat.txt","r") 
text=file1.read()
text=text+' #'
print(text)


print(datetime.datetime.now().time().__format__('%H:%M:%S'))
ser.open()
ser.write(text.encode())
ser.close()
print(datetime.datetime.now().time().__format__('%H:%M:%S'))
cv2.namedWindow("arena image",cv2.WINDOW_NORMAL)
cv2.imshow("arena image",image)
cv2.waitKey(0)
cv2.destroyAllWindows()