version = 1.0;

local pattern = "^(.-)%b()"

function table.map(t, func)
    local result = {}
    for i, v in ipairs(t) do
        result[i] = func(v)
    end
    return result
end

function table.contain(t, ele)
    for _, v in pairs(t) do
        if v == ele then
            return true;
        end
    end
    return false;
end

local function split(str, delimiter)
    local result = {};
    for match in string.gmatch(str, "[^" .. delimiter .. "]+") do
        table.insert(result, match);
    end
    return result
end

local function de_parentheses(str)
    for match in string.gmatch(str, "%((.*)%)") do
        return match;
    end
    return nil;
end

local function trim(str)
    return str:gsub("^%s+", "")
end

local function trim_count(str)
    local count = 0
    while str:sub(1, 1) == " " do
        str = str:sub(2)
        count = count + 1
    end
    return str, count
end

local function string_repeat(str, n)
    local result = ""
    for _ = 1, n do
        result = result .. str
    end
    return result
end

local function get_procedure(cmd)
    local matched = string.match(trim(cmd), pattern);
    local result = "note";
    if type(matched) ~= "nil" then
        result = matched;
    end
    return result;
end

local function break_cmd(procedure, cmd)
    local trimed, detrim_count = trim_count(cmd);
    if procedure == "scenecontrol" then
        -- scenecontrol(t,type,*param1(float),*param2(int));
        local args = split(de_parentheses(trimed), ",");
        local result = {
            tonumber(args[1]),
            args[2]
        };

        if #args == 4 then
            -- the third arg must be float number
            table.insert(result, string.format("%.2f", tonumber(args[3])))
            table.insert(result, tonumber(args[4]))
        end

        return result, detrim_count;
    end
end

local function build_cmd(procedure, args, extra, detrim_count)
    return string_repeat(" ", detrim_count) .. procedure .. "(" .. table.concat(args, ",") .. ")" .. extra .. ";";
end

local flag_property = false;

local available_sc_commands = { "trackhide", "trackshow", "trackdisplay", "redline", "arcahvdistort", "arcahvdebris", "hidegroup", "enwidencamera", "enwidenlanes" }

local function process(cmd)
    if not flag_property then
        if (cmd == "-") then
            flag_property = true;
        end
        return cmd;
    end

    local procedure = get_procedure(cmd);
    if procedure == "scenecontrol" then
        local args, detrim_count = break_cmd(procedure, cmd);
        if not table.contain(available_sc_commands, args[2]) then
            return "";
        end

        if args[2] == "hidegroup" then
            -- #1: scenecontrol(27201,hidegroup,0,256);
            -- #2: scenecontrol(27201,hidegroup,0,1);

            if args[4] > 128 then
                args[4] = 1;
            else
                args[4] = 0;
            end
            return build_cmd(procedure, args, "", detrim_count);
        end
    end

    return cmd;
end

function exec(aff)
    local cmds = split(aff, '\n');
    return table.concat(table.map(cmds, process), "\n");
end
