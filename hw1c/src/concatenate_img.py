import cv2
  
# read the images
imgs = [cv2.imread(i) for i in ['1.jpg']]


# vertically concatenates images 
# of same width 
im_v = cv2.hconcat(imgs)
  
# show the output image
cv2.imwrite('merge.jpg', im_v)