LAYER=$1
mkdir N$LAYER

cd N$LAYER
mkdir Module
mkdir PMT
mkdir resModule
mkdir resPMT
cd ../

./getinput $LAYER
python3 genQrCode.py ./N$LAYER/ Module
./getModlabels ./N$LAYER/
