# Play with it 

### Setup

```bash
[[ $0 == *zsh* ]] && setopt interactivecomments
```

### Compile

```bash
javac -d /mnt/ram/BreakingLCG Gen.java
javac -d /mnt/ram/BreakingLCG ManualGen.java
```

### Run Generators

```bash
# Tee out only lines in between #####...
# The other ones our C++ adversary will calculate
java --add-opens java.base/java.util=ALL-UNNAMED -cp /mnt/ram/BreakingLCG Gen | tee >(awk '/^#{32}/{if(p)exit;p=1;next}p' > /mnt/ram/BreakingLCG/gen.txt)
echo '<<<<<<<< >>>>>>>> <<<<<<<< >>>>>>>> <<<<<<<< >>>>>>>>'
java -cp /mnt/ram/BreakingLCG ManualGen | tee >(awk '/^#{32}/{if(p)exit;p=1;next}p' > /mnt/ram/BreakingLCG/manual_gen.txt)

```

### Check Output

```bash
cat /mnt/ram/BreakingLCG/lcg.txt
echo '<<<<<<<< >>>>>>>>'
cat /mnt/ram/BreakingLCG/gen.txt
```
