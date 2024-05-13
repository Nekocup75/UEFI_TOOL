

import os

ModuleName = 'C:/Makefile'
WorkStation = 'C:/Users/Nekocup/Desktop/edk2'

class MyWorkStation:
  def __init__(self):
    f = open (ModuleName, 'r')
    Content = f.read()
    f.close()
    # BIN_DIR
    BUILD_DIR_index = Content.find('BUILD_DIR')
    BUILD_DIR = Content[BUILD_DIR_index:].split('\n')[0].split('=')[1].strip()
    BIN_DIR_index = Content.find('BIN_DIR')
    BIN_DIR = Content[BIN_DIR_index:].split('\n')[0].split('=')[1].strip().replace('$(BUILD_DIR)',BUILD_DIR)
    self.binDir = BIN_DIR
    # STATIC_LIBRARY_FILES
    index_start = Content.find ('STATIC_LIBRARY_FILES =')
    index_end = Content.find ('INC =')
    STATIC_LIBRARY_FILES = Content[index_start:index_end]
    # STATIC_LIBRARY_FILES = STATIC_LIBRARY_FILES.replace('$(BIN_DIR)',BIN_DIR)
    # 0 is arrayName
    STATIC_LIBRARY_FILES_Array = STATIC_LIBRARY_FILES.split('\n')
    STATIC_LIBRARY_FILES_Array.remove('STATIC_LIBRARY_FILES =  \\')

    tmp = []
    tmp1 = []
    # STATIC_LIBRARY_FILES_Array.remove('')
    self.libraryArray = STATIC_LIBRARY_FILES_Array
    for index in self.libraryArray:
      if index == '':
        self.libraryArray.remove(index)
      if '$(OUTPUT_DIR)' in index:
       self.libraryArray.remove(index)
       continue
      index = index.split('\\OUTPUT\\')[0].strip().replace('\\','/')
      if index != '':
        index = index.split('$(BIN_DIR)')[1]
        tmp.append(index)
    self.libraryArray = tmp
    
    for index in self.libraryArray:
      index = index.split('/')[-1]
      if index != '':
        tmp1.append(index)
    self.clearLibrary = tmp1
    
  def WorkPath(self, root):
    self.root = root
    
  def find_files(self, fileNmaeInput):
    files = []
    InfFile = []
    for root, _, filenames in os.walk(self.root):
      for filename in filenames:
        if filename.startswith(fileNmaeInput):
          files.append(os.path.join(root, filename).replace('\\','/'))
    return files

  def find_inf(self, fileNmaeInput):
    InfFile = []
    for root, _, filenames in os.walk(self.root):
      for filename in filenames:
        if filename.startswith(fileNmaeInput):
          if 'inf' in filename:
            InfFile.append(os.path.join(root, filename).replace('\\','/'))
    return InfFile
  
  def find_Lib(self, file):
      f = open (file, 'r')
      found = 0
      tmp = []
      LibArray = []
      for line in f.readlines():
        if 'LIBRARY_CLASS' in line:
          print ("Library Name is :",line.split('LIBRARY_CLASS')[1].strip().split('|')[0].replace('=', '').strip())
        if line.strip() == "[LibraryClasses]":
          found = 1
        elif found == 1:
          if line.strip() != '':
            print(line)
          tmp.append(line.replace('\n','').strip())
          if line.strip() == '':
            break
      for index in tmp:
        if index == '':
          tmp.remove(index)
        LibArray = tmp
      f.close()
      return LibArray

if __name__ == '__main__':
  Data = MyWorkStation()
  Data.WorkPath (WorkStation)

  
  for index in Data.clearLibrary:
    print (index)
    Inf = Data.find_inf (index)
    print('-----------------------')
    print (Data.find_Lib(Inf[0]))
    print('-----------------------')
