--[[
   mips-data.lua - MIPS Data Packer

   Written in 2014 by Jack Force <jack1243star@gmail.com>

   To the extent possible under law, the author(s) have dedicated all
   copyright and related and neighboring rights to this software to the
   public domain worldwide. This software is distributed without any
   warranty.

   You should have received a copy of the CC0 Public Domain Dedication
   along with this software. If not, see
   <http://creativecommons.org/publicdomain/zero/1.0/>.
]]--

--[[

   Description
   ===========

   This is a simple program to ease the creation of dimage.bin file.

   Input file is a plain text file with white-space-separated numbers,
   either hexadecimal or decimal. The first number denotes the initial
   value of the stack pointer ($sp), and the others are the data loaded
   into the simulator memory.

   Note that the number of words to load is automatically determined in
   this program. Do not add it in the input file.

]]--

-- Return binary string representation of a 32-bit integer
function binary(i, endian)
   local result = ""
   local ls = bit32.lshift
   local mask = bit32.band
   if endian == "b" then
      result = result .. string.char(mask(ls(i, -24), 0xFF))
      result = result .. string.char(mask(ls(i, -16), 0xFF))
      result = result .. string.char(mask(ls(i, -8), 0xFF))
      result = result .. string.char(mask(ls(i, -0), 0xFF))
   elseif endian == "l" then
      result = result .. string.char(mask(ls(i, -0), 0xFF))
      result = result .. string.char(mask(ls(i, -8), 0xFF))
      result = result .. string.char(mask(ls(i, -16), 0xFF))
      result = result .. string.char(mask(ls(i, -24), 0xFF))
   else
      error("Endianness not specified")
   end
   return result
end

-- Check arguments
if #arg ~= 2 then
   print("Usage: lua mips-data.lua <input> <output>")
end
-- Get input and outpu files from command line arguments
io.input(arg[1])
io.output(arg[2])
-- Read the offset
local stackptr = io.read("*n")
io.write(binary(stackptr, "l"))
-- Read and count data
local data = {}
local num = 0
while true do
   local d = io.read("*n")
   if d == nil then break end
   table.insert(data, d)
   num = num + 1
end
-- Write number and data
io.write(binary(num, "l"))
for i,v in ipairs(data) do
   io.write(binary(v, "l"))
end
