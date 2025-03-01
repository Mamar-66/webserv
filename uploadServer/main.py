from programs.KNN import *
from programs.GetPoints import *
from programs.Create2ndMG import *
import os

image_mes = "TestOL2"

def count_files_in_directory(directory):
    all_items = os.listdir(directory)
    files = [item for item in all_items if os.path.isfile(os.path.join(directory, item))]
    return len(files)

init_clean_img(image_mes)
init_segment_detection(image_mes)
lsFinal = init_number_prediction(count_files_in_directory("./test"), [], [])
print(lsFinal)

