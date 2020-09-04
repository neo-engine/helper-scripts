# helper-scripts
Various helper scrips to convert graphics.
All scripts can be build with the supplied Makefile via a single `make`. 

Some conversion scripts automatically reduce the number of colors in the image; for some scripts a parameter `THESHOLD` may additionally specify when to colors 
are to be treated as equal (if `|red(c1)-red(c2)| + |green(c1)-green(c2)| + |blue(c1)-blue(c2)| < THRESHOLD`).
All optional arguments before the first argument explicitly given must be specified as well.

### 128x32
Converts a 128 by 32 pixel png file to an indexed raw image with 16 colors. (Used for location name backgrounds when entering a new location)

Usage: `128x32 PNG [THRESHOLD = 10]`

### ampal2p2l
Converts 6 ore more palettes as exported from AdvanceMap into a single `p2l` palette file for _neo_.

Usage: `ampal2p2l pal1 pal2 pal3 pal4 pal5 pal6 [...] output.p2l`

### csvjoin
Merges two `.csv` files, optionally dropping columns. Which columns to select is specified by a string consisting of 
one the characters `1`, `2`, `b`, `r`, `x`; where the i-th character describes
* `1`: use the i-th column from the first file, discard the i-th column from the second file
* `2`: use the i-th column from the second file, discard the i-th column from the first file
* `b`: use first the i-th column from the first file, then the i-th column from the second file
* `r`: use first the i-th column from the second file, then the i-th column from the first file
* `x`: discard both the i-th column of the first file and the i-th column of the second file

Usage: `csvjoin file1.csv file2.csv mergestring`

### csvjoin2
As csvjoin, but using a semicolon (`;`) as a seperator.

### normalizepkmnicon
Crops a 40 by 30 pixel png file to a 32 by 32 png file; producing a file with the name `icon###.png` if the name of the original file was 
found in the specified name table. (Used to convert gen6-pkmn icons named with the pkmn's name to gen5-style pkmn icons with easier usable names.)

Usage: `normalizepkmnicon NAMETABLE_PATH in.png [SOURCE_HEIGHT = 30] [SOURCE_WIDTH = 40] [TARGET_HEIGHT = 32] [TARGET_WIDTH = 32]`
(`NAMETABLE_PATH` is a path to a csv file containing in line `i` the `i+1st`-name.)

### pkmnicon2rsd
Converts a 64x128 png icon to a rsd file (a raw format for multi-frame sprites: first palette, then sprite data (tiled))
Arguments: `icon###[.*].png`, where ### is the id of the corresponding pkmn

Outputs the file into folder (###)/30 (padded with zeros to a 2 character name)
(Downscales 128x256 icons to two 32x32 icons by combining 4 pixels into one using majority)

The sprite may not contain more than 16 colors. The program automatically combines
similar colors and will do so very aggressively to enforce this color limit.

Usage: `pkmnicon2rsd icon###[.*].png [HEIGHT = 64] [WIDTH = 64] [NUMFRAMES = 1] [THRESHOLD = 10]`

### pkmnsprite2raw
Converts a 96 by 96 pixel (or 192 by 192 pixel) sprite to the raw representation used by _neo_.
Arguments: `###[.*].png`, where ### is the id of the corresponding pkmn

Outputs the file into folder (###)/30 (padded with zeros to a 2 character name)
(Downscales 128x256 icons to two 32x32 icons by combining 4 pixels into one using majority)

The sprite may not contain more than 16 colors. The program automatically combines
similar colors and will do so very aggressively to enforce this color limit.

Usage: `pkmnsprite2raw ###[.*].png [NUMFRAMES = 1] [THRESHOLD = 10]`

### plat2raw 
Converts a 128 by 64 pixel png file to an indexed raw image with 16 colors. (Used for platforms during a battle)

Usage: `plat2raw PNG [THRESHOLD = 10]`

### png2raw
Converts a 256x192 png file to an indexed, raw representation used for _neo_ (does not tile the image). Allows in any case not more that 256 colors.

Usage: `png2raw input.png [FIRST_PALETTE_IDX = 1] [MAX_COLORS = 235] [THRESHOLD = 5]

### raw2png
Converts a *.raw (non-tiled) file to a *.png file.

Tries to guess the dimensions of the original png if they're not given (out of 256x192, 64x64, 32x64, 32x32, 16x(#img length / 4))

Usage: `raw2png input.raw TILE_CNT COLOR_CNT [WIDTH] [HEIGHT = 4 * TILE_CNT / WIDTH]

### raw2sprite
Reads a `*.rsd` tiled sprite and prints it to the console.

Usage: `raw2sprite input.rsd`

### sprite2raw
Converts a sprite to a rsd file

Outputs the file into folder (###)/30 (padded with zeros to a 2 character name) if ###
exists; otherwise outputs to working dir

The sprite may not contain more than 16 colors. The program automatically combines
similar colors and will do so very aggressively to enforce this color limit.

Usage: `sprite2raw [icon###(where ### is the id of the corresponding sprite)][.*].png [HEIGHT = 32] [WIDTH = 32] [NUM_FRAMES = 1 (if > 0, generates *rsd, ow generates *raw) [THERSHOLD = 10] [TRANSPARENT_COLOR = (color at (0,0)] [COLOR1] [COLOR2] ...`

