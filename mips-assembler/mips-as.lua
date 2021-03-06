--[[
   mips-as.lua - MIPS Assembler

   Written in 2014 by Jack Force <jack1243star@gmail.com>

   To the extent possible under law, the author(s) have dedicated all
   copyright and related and neighboring rights to this software to the
   public domain worldwide. This software is distributed without any
   warranty.

   You should have received a copy of the CC0 Public Domain Dedication
   along with this software. If not, see
   <http://creativecommons.org/publicdomain/zero/1.0/>.
--]]

-- Check empty or comment only string
function empty(s)
   local first = string.match(s, "%g")
   if first == nil or first == "#" or first == ";" then
      return true
   else
      return false
   end
end

-- Return label from string, or nil when there is no label
function getlabel(s)
   local labelwithcolon = string.match(s, "%g+%s*:")
   if labelwithcolon then
      return string.sub(labelwithcolon, 1, -2)
   else
      return nil
   end
end

-- Return instruction mnemonic from string
function getinstr(s)
   if string.find(s, ":") then
      return string.match(s, ":%s*(%a+)")
   else
      return string.match(s, "%s*(%a+)")
   end
end

-- Return rd, rs and rt from string
function getdst(s)
   local rd = tonumber(string.match(s, "$(%d+)%s*"))
   local rs = tonumber(string.match(s, ",%s*$(%d+)"))
   local rt = tonumber(string.match(s, ",[^,]*,%s*$(%d+)"))
   return rd, rs, rt
end

-- Return rd, rt and C from string
function getdtc(s)
   local rd = tonumber(string.match(s, "$(%d+)%s*"))
   local rt = tonumber(string.match(s, ",%s*$(%d+)"))
   local c = tonumber(string.match(s, ",[^,]*,%s*(%g+)"))
   return rd, rt, c
end

-- Return rt, C and rs from string
function gettcs(s)
   local rt = tonumber(string.match(s, "$(%d+)%s*"))
   local c = tonumber(string.match(s, "(%g+)%("))
   local rs = tonumber(string.match(s, "%($(%d+)"))
   return rt, c, rs
end

-- Return rs, rt and label from string
function getstl(s)
   local rs = tonumber(string.match(s, "$(%d+)%s*"))
   local rt = tonumber(string.match(s, ",%s*$(%d+)"))
   local label = string.match(s, ",[^,]*,%s*(%g+)")
   return rs, rt, label
end

-- Return 32-bit integer of an R-Type Instruction
function rinst(opcode, rs, rt, rd, shamt, funct)
   -- Check bounds
   assert(opcode <= 0x3F)
   assert(rs <= 0x1F)
   assert(rt <= 0x1F)
   assert(rd <= 0x1F)
   assert(shamt <= 0x1F)
   assert(funct <= 0x3F)

   local tmp = 0xFFFFFFFF
   local buf = 0xFFFFFFFF

   -- Write into buffer
   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, opcode)
   tmp = bit32.lshift(tmp, 26)
   tmp = bit32.bor(tmp, 0x03FFFFFF)
   buf = bit32.band(buf, tmp)

   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, rs)
   tmp = bit32.lshift(tmp, 21)
   tmp = bit32.bor(tmp, 0xFC1FFFFF)
   buf = bit32.band(buf, tmp)

   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, rt)
   tmp = bit32.lshift(tmp, 16)
   tmp = bit32.bor(tmp, 0xFFE0FFFF)
   buf = bit32.band(buf, tmp)

   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, rd)
   tmp = bit32.lshift(tmp, 11)
   tmp = bit32.bor(tmp, 0xFFFF07FF)
   buf = bit32.band(buf, tmp)

   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, shamt)
   tmp = bit32.lshift(tmp, 6)
   tmp = bit32.bor(tmp, 0xFFFFF83F)
   buf = bit32.band(buf, tmp)

   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, funct)
   tmp = bit32.lshift(tmp, 0)
   tmp = bit32.bor(tmp, 0xFFFFFFC0)
   buf = bit32.band(buf, tmp)

   return buf
end

-- Return 32-bit integer of an I-Type Instruction
function iinst(opcode, rs, rt, c)
   -- Check bounds
   assert(opcode <= 0x3F)
   assert(rs <= 0x1F)
   assert(rt <= 0x1F)

   local tmp = 0xFFFFFFFF
   local buf = 0xFFFFFFFF

   -- Write into buffer
   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, opcode)
   tmp = bit32.lshift(tmp, 26)
   tmp = bit32.bor(tmp, 0x03FFFFFF)
   buf = bit32.band(buf, tmp)

   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, rs)
   tmp = bit32.lshift(tmp, 21)
   tmp = bit32.bor(tmp, 0xFC1FFFFF)
   buf = bit32.band(buf, tmp)

   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, rt)
   tmp = bit32.lshift(tmp, 16)
   tmp = bit32.bor(tmp, 0xFFE0FFFF)
   buf = bit32.band(buf, tmp)

   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, c)
   tmp = bit32.lshift(tmp, 0)
   tmp = bit32.bor(tmp, 0xFFFF0000)
   buf = bit32.band(buf, tmp)

   return buf
end

-- Return 32-bit integer of a J-Type Instruction
function jinst(opcode, c)
   -- Check bounds
   assert(opcode <= 0x3F)
   assert(c <= 0x03FFFFFF)

   local tmp = 0xFFFFFFFF
   local buf = 0xFFFFFFFF

   -- Write into buffer
   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, opcode)
   tmp = bit32.lshift(tmp, 26)
   tmp = bit32.bor(tmp, 0x03FFFFFF)
   buf = bit32.band(buf, tmp)

   tmp = 0xFFFFFFFF
   tmp = bit32.band(tmp, c)
   tmp = bit32.lshift(tmp, 0)
   tmp = bit32.bor(tmp, 0xFC000000)
   buf = bit32.band(buf, tmp)

   return buf
end

-- MIPS instructions
instruction = {
   -- R-Type Instructions
   ["add"] = function (s)
      local rd, rs, rt = getdst(s)
      return rinst(0x00, rs, rt, rd, 0x00, 0x20)
   end,
   ["sub"] = function (s)
      local rd, rs, rt = getdst(s)
      return rinst(0x00, rs, rt, rd, 0x00, 0x22)
   end,
   ["and"] = function (s)
      local rd, rs, rt = getdst(s)
      return rinst(0x00, rs, rt, rd, 0x00, 0x24)
   end,
   ["or"] = function (s)
      local rd, rs, rt = getdst(s)
      return rinst(0x00, rs, rt, rd, 0x00, 0x25)
   end,
   ["xor"] = function (s)
      local rd, rs, rt = getdst(s)
      return rinst(0x00, rs, rt, rd, 0x00, 0x26)
   end,
   ["nor"] = function (s)
      local rd, rs, rt = getdst(s)
      return rinst(0x00, rs, rt, rd, 0x00, 0x27)
   end,
   ["nand"] = function (s)
      local rd, rs, rt = getdst(s)
      return rinst(0x00, rs, rt, rd, 0x00, 0x28)
   end,
   ["slt"] = function (s)
      local rd, rs, rt = getdst(s)
      return rinst(0x00, rs, rt, rd, 0x00, 0x2A)
   end,
   ["sll"] = function (s)
      local rd, rt, c = getdtc(s)
      return rinst(0x00, 0x00, rt, rd, c, 0x00)
   end,
   ["srl"] = function (s)
      local rd, rt, c = getdtc(s)
      return rinst(0x00, 0x00, rt, rd, c, 0x02)
   end,
   ["sra"] = function (s)
      local rd, rt, c = getdtc(s)
      return rinst(0x00, 0x00, rt, rd, c, 0x03)
   end,
   ["jr"] = function (s)
      local rs = getdst(s)
      return rinst(0x00, rs, 0x00, 0x00, 0x00, 0x08)
   end,
   -- I-Type Instructions
   ["addi"] = function (s)
      local rt, rs, c = getdtc(s)
      return iinst(0x08, rs, rt, c)
   end,
   ["lw"] = function (s)
      local rt, c, rs = gettcs(s)
      return iinst(0x23, rs, rt, c)
   end,
   ["lh"] = function (s)
      local rt, c, rs = gettcs(s)
      return iinst(0x21, rs, rt, c)
   end,
   ["lhu"] = function (s)
      local rt, c, rs = gettcs(s)
      return iinst(0x25, rs, rt, c)
   end,
   ["lb"] = function (s)
      local rt, c, rs = gettcs(s)
      return iinst(0x20, rs, rt, c)
   end,
   ["lbu"] = function (s)
      local rt, c, rs = gettcs(s)
      return iinst(0x24, rs, rt, c)
   end,
   ["sw"] = function (s)
      local rt, c, rs = gettcs(s)
      return iinst(0x2B, rs, rt, c)
   end,
   ["sh"] = function (s)
      local rt, c, rs = gettcs(s)
      return iinst(0x29, rs, rt, c)
   end,
   ["sb"] = function (s)
      local rt, c, rs = gettcs(s)
      return iinst(0x28, rs, rt, c)
   end,
   ["lui"] = function (s)
      local rt = tonumber(string.match(s, "$(%d+)%s*"))
      local c = tonumber(string.match(s, ",%s*(%w+)"))
      return iinst(0x0F, 0x00, rt, c)
   end,
   ["andi"] = function (s)
      local rt, rs, c = getdtc(s)
      return iinst(0x0C, rs, rt, c)
   end,
   ["ori"] = function (s)
      local rt, rs, c = getdtc(s)
      return iinst(0x0D, rs, rt, c)
   end,
   ["nori"] = function (s)
      local rt, rs, c = getdtc(s)
      return iinst(0x0E, rs, rt, c)
   end,
   ["slti"] = function (s)
      local rt, rs, c = getdtc(s)
      return iinst(0x0A, rs, rt, c)
   end,
   ["beq"] = function (s, labels, current)
      local rs, rt, label = getstl(s)
      -- Lookup label name
      local offset = labels[label]
      if not offset then
         error(string.format('Label "%s" not found', label))
      end
      return iinst(0x04, rs, rt, offset - current - 1)
   end,
   ["bne"] = function (s, labels, current)
      local rs, rt, label = getstl(s)
      -- Lookup label name
      local offset = labels[label]
      if not offset then
         error(string.format('Label "%s" not found', label))
      end
      return iinst(0x05, rs, rt, offset - current - 1)
   end,
   -- J-Type Instructions (Not implemented)
   ["j"] = function (s, labels, current, startaddr)
      local label = string.match(s, "j%s+(%g+)")
      local offset = labels[label]
      if not offset then
         error(string.format('Label "%s" not found', label))
      end
      return jinst(0x02, (startaddr/4)+offset)
   end,
   ["jal"] = function (s, labels, current, startaddr)
      local label = string.match(s, "jal%s+(%g+)")
      local offset = labels[label]
      if not offset then
         error(string.format('Label "%s" not found', label))
      end
      return jinst(0x03, (startaddr/4)+offset)
   end,
   -- Specialized Instruction
   ["halt"] = function (s)
      return iinst(0x3F, 0x00, 0x00, 0x0000)
   end
}

-- MIPS register names
register = {
   ["$zero"] = "$0",
   ["$at"] = "$1",
   ["$v0"] = "$2",
   ["$v1"] = "$3",
   ["$a0"] = "$4",
   ["$a1"] = "$5",
   ["$a2"] = "$6",
   ["$a3"] = "$7",
   ["$t0"] = "$8",
   ["$t1"] = "$9",
   ["$t2"] = "$10",
   ["$t3"] = "$11",
   ["$t4"] = "$12",
   ["$t5"] = "$13",
   ["$t6"] = "$14",
   ["$t7"] = "$15",
   ["$s0"] = "$16",
   ["$s1"] = "$17",
   ["$s2"] = "$18",
   ["$s3"] = "$19",
   ["$s4"] = "$20",
   ["$s5"] = "$21",
   ["$s6"] = "$22",
   ["$s7"] = "$23",
   ["$t8"] = "$24",
   ["$t9"] = "$25",
   ["$k0"] = "$26",
   ["$k1"] = "$27",
   ["$gp"] = "$28",
   ["$sp"] = "$29",
   ["$fp"] = "$30",
   ["$ra"] = "$31",
}

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

-- Create MIPS binary from assembly
function mipsas(inputfile, outputfile, startaddr)
   -- Label table
   local labels = {}
   -- Offset from start address
   local offset = 0
   -- Start address
   local startaddr = startaddr or 0;
   -- Number of words
   local number
   -- List of instructions
   local list = {}
   -- Instruction implementation
   local inst = instruction

   -- Strip comment and translate register name
   io.input(inputfile)
   local srclist = {}
   for s in io.lines() do
      if not empty(s) then
         -- Strip comment
         local stripped = string.match(s, "([^#;]*)").."\n"
         -- Translate register name
         local t1 = string.gsub(stripped, "$zero", "$0")
         local t2 = string.gsub(t1, "$%a%w", register)
         table.insert(srclist, t2)
      end
   end
   -- Write to temp file
   local src = io.tmpfile()
   for i,v in pairs(srclist) do src:write(v) end
   src:flush()

   -- Create label table
   src:seek("set")
   for s in src:lines() do
      -- Get label
      local label = getlabel(s)
      if label then labels[label] = offset end
      -- Increment offset
      if getinstr(s) then offset = offset + 1 end
   end
   -- Record number of words
   number = offset

   -- Process instructions
   offset = 0
   src:seek("set")
   for s in src:lines() do
      local i = getinstr(s)
      if i then
         if inst[i] then
            -- Write to list
            table.insert(list,
                         inst[i](s, labels, offset, startaddr))
         else
            error(string.format('"%s" not implemented', i))
         end
         offset = offset + 1
      end
   end

   -- Write list as binary
   local out = io.open(outputfile, "wb")
   out:write(binary(startaddr, "l"))
   out:write(binary(number, "l"))
   for i=1,#list do
      out:write(binary(list[i], "l"))
   end
   out:close()
end

-- Start assembler
if #arg ~= 3 then
   print(
      "Usage:",
      "lua mips-as.lua <input-file> <output-file> <start-address>")
else
   local input = arg[1]
   local output = arg[2]
   local startaddr = arg[3]
   mipsas(input, output, tonumber(startaddr))
end
