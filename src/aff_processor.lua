function table.length(t)
    local count = 0
    for _ in pairs(t) do
        count = count + 1
    end
    return count
end

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

local function clone(object)
    local lookup_table = {}
    local function _copy(obj)
        if type(obj) ~= "table" then
            return obj
        elseif lookup_table[obj] then
            return lookup_table[obj]
        end
        local new_table = {}
        lookup_table[obj] = new_table
        for key, value in pairs(obj) do
            new_table[_copy(key)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(obj))
    end
    return _copy(object)
end

local function split(str, delimiter)
    local result = {};
    if str == nil then
        return {};
    end
    for match in string.gmatch(str, "[^" .. delimiter .. "]+") do
        table.insert(result, match);
    end
    return result
end

local function de_parentheses(str)
    local rst = string.match(str, "%((.-)%)");
    if rst ~= nil then
        return rst;
    end
    return nil;
end

local function get_arctaps(str)
    local rst = string.match(str, "arc%(.*%)%[(.*)%];");
    if rst ~= nil then
        return rst;
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
    if trim(cmd) == "};" then
        return "timinggroupend";
    end

    if string.sub(trim(cmd), 1, 1) == "(" then
        return "note";
    end

    local matched = string.match(trim(cmd), "^(.-)%b()");
    if matched ~= nil then
        return matched;
    end
    return nil;
end

local function break_cmd(procedure, cmd)
    local trimed, detrim_count = trim_count(cmd);

    if procedure == "scenecontrol" then
        local args = split(de_parentheses(trimed), ",");
        local result = {
            tonumber(args[1]),
            args[2]
        };

        if #args == 4 and tonumber(args[3]) ~= nil and tonumber(args[4]) ~= nil then
            table.insert(result, tonumber(args[3]))
            table.insert(result, tonumber(args[4]))
        end

        return result, detrim_count;
    elseif procedure == "note" then
        -- (3467,4);
        local args = split(de_parentheses(trimed), ",");
        return {
            tonumber(args[1]),
            tonumber(args[2])
        }, detrim_count;
    elseif procedure == "hold" then
        local args = split(de_parentheses(trimed), ",");
        return {
            tonumber(args[1]),
            tonumber(args[2]),
            tonumber(args[3])
        }, detrim_count;
    elseif procedure == "arc" then
        local args = split(de_parentheses(trimed), ",");
        if #args == 9 then
            table.insert(args, 9, "none");
        end
        local arctaps = split(get_arctaps(trimed), ",");
        local result = {
            tonumber(args[1]),
            tonumber(args[2]),
            tonumber(args[3]),
            tonumber(args[4]),
            args[5],
            tonumber(args[6]),
            tonumber(args[7]),
            tonumber(args[8]),
            args[9],
            args[10],
            nil
        };

        if next(arctaps) ~= nil then
            -- there are arctaps
            local tb_arctaps = {};
            for _, v in ipairs(arctaps) do
                table.insert(tb_arctaps, tonumber(de_parentheses(v)));
            end

            result[11] = tb_arctaps;
        end
        return result, detrim_count;
    elseif procedure == "timing" then
        local args = split(de_parentheses(trimed), ",");
        return {
            tonumber(args[1]),
            tonumber(args[2]),
            tonumber(args[3])
        }, detrim_count;
    elseif procedure == "camera" then
        local args = split(de_parentheses(trimed), ",");
        return {
            tonumber(args[1]),
            tonumber(args[2]),
            tonumber(args[3]),
            tonumber(args[4]),
            tonumber(args[5]),
            tonumber(args[6]),
            tonumber(args[7]),
            args[8],
            tonumber(args[9])
        }, detrim_count;
    elseif procedure == "timinggroup" then
        local args = split(de_parentheses(trimed), ",");
        return args, detrim_count;
    else
        return nil, detrim_count;
    end
end

local function build_cmd(procedure, args, extra, detrim_count)
    return string_repeat(" ", detrim_count) .. procedure .. "(" .. table.concat(args, ",") .. ")" .. extra .. ";";
end

local function build_timing_group(args, detrim_count)
    return string_repeat(" ", detrim_count) .. "timinggroup" .. "(" .. table.concat(args, "_") .. "){";
end

local function build_note(args, detrim_count)
    return string_repeat(" ", detrim_count) .. "(" .. table.concat(args, ",") .. ");";
end

local function build_arc(args, detrim_count)
    local basic_args = table.pack(table.unpack(args, 1, 10));
    if args[11] == nil then
        return build_cmd("arc", basic_args, "", detrim_count);
    end
    local taps = {};
    for _, v in ipairs(args[11]) do
        table.insert(taps, "arctap(" .. tostring(v) .. ")");
    end
    local extra = "[" .. table.concat(taps, ",") .. "]";
    return build_cmd("arc", basic_args, extra, detrim_count);
end

local available_sc_commands = { "trackhide", "trackshow", "trackdisplay", "redline", "arcahvdistort", "arcahvdebris",
                                "hidegroup", "enwidencamera", "enwidenlanes" }

local invalid_cmd = "###invalid_sc###";

flag_property = false;

arc_color_conv_cnt = 0;
invalid_sc_conv_cnt = 0;
invalid_tg_conv_cnt = 0;

local function process(cmd)
    if not flag_property then
        if (cmd == "-") then
            flag_property = true;
        end
        return cmd;
    end

    local procedure = get_procedure(cmd);

    if procedure == "timing" then
        local args, detrim_count = break_cmd(procedure, cmd);
        assert(args ~= nil);

        if args[3] == 0 then
            args[3] = 1;
        end

        -- force parameter 2 and 3 to be '%.2f' formatted
        args[2] = string.format("%.2f", args[2]);
        args[3] = string.format("%.2f", args[3]);

        return build_cmd(procedure, args, "", detrim_count);
    elseif procedure == "note" then
        local args, detrim_count = break_cmd(procedure, cmd);
        assert(args ~= nil);

        if math.tointeger(args[2]) ~= nil then
            if args[2] < 0 or args[2] > 5 then
                return invalid_cmd;
            end
        end

        return build_note(args, detrim_count);
    elseif procedure == "hold" then
        local args, detrim_count = break_cmd(procedure, cmd);
        assert(args ~= nil);

        if math.tointeger(args[3]) ~= nil then
            if args[3] < 0 or args[3] > 5 then
                -- args[3] = string.format("%.2f", (args[3] + 0.5) / 2);
                return invalid_cmd;
            end
        end

        return build_cmd(procedure, args, "", detrim_count);
    elseif procedure == "arc" then
        local args, detrim_count = break_cmd(procedure, cmd);
        assert(args ~= nil);

        if 0 > args[8] or args[8] > 2 then
            args[8] = 2; -- convert to green arc if overflow
            arc_color_conv_cnt = arc_color_conv_cnt + 1;
        end

        if args[9] == "" then
            args[9] = "none";
        end

        for wav in string.gmatch(args[9], "(.*)_wav") do
            if wav ~= nil then
                :: continue ::
            end

            local wav_file = io.open(out_proj_path .. wav .. ".wav", "r");
            if wav_file then
                io.close(wav_file);
            else
                args[9] = "none";
            end
            break
        end

        for wav in string.gmatch(args[9], "(.*)%.wav") do
            if wav ~= nil then
                :: continue ::
            end

            local wav_file = io.open(out_proj_path .. wav .. ".wav", "r");
            if wav_file then
                args[9] = wav .. "_wav";
                io.close(wav_file);
            else
                args[9] = "none";
            end
            break
        end
        args[3] = string.format("%.2f", args[3]);
        args[4] = string.format("%.2f", args[4]);
        args[6] = string.format("%.2f", args[6]);
        args[7] = string.format("%.2f", args[7]);

        return build_arc(args, detrim_count);
    elseif procedure == "camera" then
        if flag_no_camera then
            return invalid_cmd;
        end

        local args, detrim_count = break_cmd(procedure, cmd);
        assert(args ~= nil);

        args[2] = string.format("%.2f", args[2]);
        args[3] = string.format("%.2f", args[3]);
        args[4] = string.format("%.2f", args[4]);
        args[5] = string.format("%.2f", args[5]);
        args[6] = string.format("%.2f", args[6]);
        args[7] = string.format("%.2f", args[7]);

        return build_cmd(procedure, args, "", detrim_count);
    elseif procedure == "scenecontrol" then
        local args, detrim_count = break_cmd(procedure, cmd);
        assert(args ~= nil);

        if not table.contain(available_sc_commands, args[2]) then
            invalid_sc_conv_cnt = invalid_sc_conv_cnt + 1;
            return invalid_cmd;
        end

        if #args == 4 then
            if args[3] == "trackdisplay" then
                -- in ArcCreate, 'trackdisplay' time is counted in millisecond, but in Arcaea it's in second
                args[3] = args[3] / 1000.0
            end

            args[3] = string.format("%.2f", args[3]);
        end

        return build_cmd(procedure, args, "", detrim_count);
    elseif procedure == "timinggroup" then
        local args, detrim_count = break_cmd(procedure, cmd);
        assert(args ~= nil);

        local result = {};

        for _, v in ipairs(args) do
            if table.contain({
                "noinput",
                "fadingholds"
            }, v)
            then
                table.insert(result, v);
            else
                for x in string.gmatch(v, "anglex=(-?%d+)") do
                    if x ~= nil and tonumber(x) ~= nil then
                        x = tonumber(x)
                        if x < 0 then
                            x = 360 + x;
                        end
                        table.insert(result, "anglex" .. math.floor(x * 10));
                    end
                end
                for y in string.gmatch(v, "angley=(-?%d+)") do
                    if y ~= nil and tonumber(y) ~= nil then
                        y = tonumber(y)
                        if y < 0 then
                            y = 360 + y;
                        end
                        table.insert(result, "angley" .. math.floor(y * 10));
                    end
                end
            end
        end

        invalid_tg_conv_cnt = invalid_tg_conv_cnt + #args - #result;

        return build_timing_group(result, detrim_count);
    elseif procedure == nil then
        return invalid_cmd;
    end

    return cmd;
end

function exec(aff)
    flag_property = false;
    arc_color_conv_cnt = 0;
    invalid_sc_conv_cnt = 0;
    invalid_tg_conv_cnt = 0;

    local cmds = split(aff, '\n');
    local result = table.map(cmds, process);

    if arc_color_conv_cnt > 0 then
        log_to_file("[LUA WARN] Fixing color for " .. arc_color_conv_cnt .. " arcs");
    end
    if invalid_sc_conv_cnt > 0 then
        log_to_file("[LUA WARN] Removing " .. invalid_sc_conv_cnt .. " invalid scene controls");
    end
    if invalid_tg_conv_cnt > 0 then
        log_to_file("[LUA WARN] Removing " .. invalid_tg_conv_cnt .. " invalid timing groups");
    end

    local rst_aff = "";
    for i, v in ipairs(result) do
        if v ~= invalid_cmd then
            rst_aff = rst_aff .. result[i] .. "\n";
        end
    end
    return rst_aff;
end

version = 1.0;
