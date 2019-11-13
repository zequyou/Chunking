//
// Created by developer on 11/3/19.
//

#ifndef CHUNKING_H
#define CHUNKING_H

#include <bits/stdc++.h>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <iostream>

using namespace std;

class chunking {
public:
    static const uint32_t RAB_POLYNOMIAL_CONST = 153191;
    static const uint64_t POLY_MASK = 0xffffffffffULL;
    static const uint32_t CHUNK_SIZE_MAX = 6 * 1024;
    static const uint32_t CHUNK_SIZE_MIN = 3 * 1024 + 512;
    static const uint32_t CHUNK_SIZE_AVE = 4 * 1024;
    static const uint32_t CHUNK_JUMP_LEN = CHUNK_SIZE_MIN - 64;

private:
    const uint64_t out[256] = {
            0ul, 828220983169ul, 556930338562ul, 285639693955ul,
            14349049348ul, 842570032517ul, 571279387910ul, 299988743303ul,
            28698098696ul, 856919081865ul, 585628437258ul, 314337792651ul,
            43047148044ul, 871268131213ul, 599977486606ul, 328686841999ul,
            57396197392ul, 885617180561ul, 614326535954ul, 343035891347ul,
            71745246740ul, 899966229909ul, 628675585302ul, 357384940695ul,
            86094296088ul, 914315279257ul, 643024634650ul, 371733990043ul,
            100443345436ul, 928664328605ul, 657373683998ul, 386083039391ul,
            114792394784ul, 943013377953ul, 671722733346ul, 400432088739ul,
            129141444132ul, 957362427301ul, 686071782694ul, 414781138087ul,
            143490493480ul, 971711476649ul, 700420832042ul, 429130187435ul,
            157839542828ul, 986060525997ul, 714769881390ul, 443479236783ul,
            172188592176ul, 1000409575345ul, 729118930738ul, 457828286131ul,
            186537641524ul, 1014758624693ul, 743467980086ul, 472177335479ul,
            200886690872ul, 1029107674041ul, 757817029434ul, 486526384827ul,
            215235740220ul, 1043456723389ul, 772166078782ul, 500875434175ul,
            229584789568ul, 1057805772737ul, 786515128130ul, 515224483523ul,
            243933838916ul, 1072154822085ul, 800864177478ul, 529573532871ul,
            258282888264ul, 1086503871433ul, 815213226826ul, 543922582219ul,
            272631937612ul, 1341293005ul, 829562276174ul, 558271631567ul,
            286980986960ul, 15690342353ul, 843911325522ul, 572620680915ul,
            301330036308ul, 30039391701ul, 858260374870ul, 586969730263ul,
            315679085656ul, 44388441049ul, 872609424218ul, 601318779611ul,
            330028135004ul, 58737490397ul, 886958473566ul, 615667828959ul,
            344377184352ul, 73086539745ul, 901307522914ul, 630016878307ul,
            358726233700ul, 87435589093ul, 915656572262ul, 644365927655ul,
            373075283048ul, 101784638441ul, 930005621610ul, 658714977003ul,
            387424332396ul, 116133687789ul, 944354670958ul, 673064026351ul,
            401773381744ul, 130482737137ul, 958703720306ul, 687413075699ul,
            416122431092ul, 144831786485ul, 973052769654ul, 701762125047ul,
            430471480440ul, 159180835833ul, 987401819002ul, 716111174395ul,
            444820529788ul, 173529885181ul, 1001750868350ul, 730460223743ul,
            459169579136ul, 187878934529ul, 1016099917698ul, 744809273091ul,
            473518628484ul, 202227983877ul, 1030448967046ul, 759158322439ul,
            487867677832ul, 216577033225ul, 1044798016394ul, 773507371787ul,
            502216727180ul, 230926082573ul, 1059147065742ul, 787856421135ul,
            516565776528ul, 245275131921ul, 1073496115090ul, 802205470483ul,
            530914825876ul, 259624181269ul, 1087845164438ul, 816554519831ul,
            545263875224ul, 273973230617ul, 2682586010ul, 830903569179ul,
            559612924572ul, 288322279965ul, 17031635358ul, 845252618527ul,
            573961973920ul, 302671329313ul, 31380684706ul, 859601667875ul,
            588311023268ul, 317020378661ul, 45729734054ul, 873950717223ul,
            602660072616ul, 331369428009ul, 60078783402ul, 888299766571ul,
            617009121964ul, 345718477357ul, 74427832750ul, 902648815919ul,
            631358171312ul, 360067526705ul, 88776882098ul, 916997865267ul,
            645707220660ul, 374416576053ul, 103125931446ul, 931346914615ul,
            660056270008ul, 388765625401ul, 117474980794ul, 945695963963ul,
            674405319356ul, 403114674749ul, 131824030142ul, 960045013311ul,
            688754368704ul, 417463724097ul, 146173079490ul, 974394062659ul,
            703103418052ul, 431812773445ul, 160522128838ul, 988743112007ul,
            717452467400ul, 446161822793ul, 174871178186ul, 1003092161355ul,
            731801516748ul, 460510872141ul, 189220227534ul, 1017441210703ul,
            746150566096ul, 474859921489ul, 203569276882ul, 1031790260051ul,
            760499615444ul, 489208970837ul, 217918326230ul, 1046139309399ul,
            774848664792ul, 503558020185ul, 232267375578ul, 1060488358747ul,
            789197714140ul, 517907069533ul, 246616424926ul, 1074837408095ul,
            803546763488ul, 532256118881ul, 260965474274ul, 1089186457443ul,
            817895812836ul, 546605168229ul, 275314523622ul, 4023879015ul,
            832244862184ul, 560954217577ul, 289663572970ul, 18372928363ul,
            846593911532ul, 575303266925ul, 304012622318ul, 32721977711ul,
            860942960880ul, 589652316273ul, 318361671666ul, 47071027059ul,
            875292010228ul, 604001365621ul, 332710721014ul, 61420076407ul,
            889641059576ul, 618350414969ul, 347059770362ul, 75769125755ul,
            903990108924ul, 632699464317ul, 361408819710ul, 90118175103ul,
    };
    const uint64_t ir[256] = {
            1ul, 10164799872ul, 7444697855ul, 4724595838ul,
            10594428413ul, 12169293692ul, 9449191675ul, 6729089658ul,
            8303954937ul, 5583852920ul, 11453685495ul, 13028550774ul,
            10308448757ul, 7588346740ul, 4868244723ul, 10738077298ul,
            12312942577ul, 9592840560ul, 11167705839ul, 12742571118ul,
            10022469101ul, 7302367084ul, 13172199659ul, 10452097642ul,
            12026962921ul, 17896795496ul, 10881726183ul, 8161624166ul,
            9736489445ul, 15606322020ul, 12886220003ul, 10166117986ul,
            11740983265ul, 13315848544ul, 10595746527ul, 7875644510ul,
            9450509789ul, 6730407772ul, 8305273051ul, 9880138330ul,
            7160036313ul, 8734901592ul, 6014799575ul, 11884632150ul,
            13459497429ul, 6444428116ul, 8019293395ul, 13889125970ul,
            11169023953ul, 8448921936ul, 14318754511ul, 7303685198ul,
            8878550477ul, 14748383052ul, 12028281035ul, 9308179018ul,
            10883044297ul, 12457909576ul, 9737807559ul, 7017705542ul,
            12887538117ul, 14462403396ul, 11742301379ul, 13317166658ul,
            10597064641ul, 7876962624ul, 13746795199ul, 15321660478ul,
            12601558461ul, 9881456444ul, 2866387131ul, 8736219706ul,
            10311084985ul, 7590982968ul, 9165848247ul, 15035680822ul,
            8020611509ul, 5300509492ul, 11170342067ul, 8450240050ul,
            10025105329ul, 15894937904ul, 13174835887ul, 6159766574ul,
            7734631853ul, 13604464428ul, 10884362411ul, 8164260394ul,
            14034092969ul, 11313990952ul, 8593888935ul, 10168754214ul,
            11743619493ul, 9023517476ul, 14893350051ul, 12173248034ul,
            9453146017ul, 11028011296ul, 8307909279ul, 14177741854ul,
            15752607133ul, 13032505116ul, 10312403099ul, 11887268378ul,
            9167166361ul, 6447064344ul, 12316896919ul, 9596794902ul,
            6876692885ul, 12746525460ul, 10026423443ul, 7306321426ul,
            8881186705ul, 14751019280ul, 7735949967ul, 5015847950ul,
            10885680525ul, 12460545804ul, 9740443787ul, 11315309066ul,
            12890174345ul, 5875105032ul, 11744937607ul, 13319802886ul,
            10599700869ul, 12174566148ul, 9454464131ul, 11029329410ul,
            12604194689ul, 9884092672ul, 11458957951ul, 17328790526ul,
            14608688509ul, 7593619196ul, 13463451771ul, 10743349754ul,
            12318215033ul, 13893080312ul, 11172978295ul, 8452876278ul,
            5732774261ul, 11602606836ul, 8882504819ul, 6162402802ul,
            12032235377ul, 9312133360ul, 6592031343ul, 8166896622ul,
            9741761901ul, 7021659884ul, 12891492459ul, 14466357738ul,
            7451288425ul, 9026153704ul, 6306051687ul, 12175884262ul,
            13750749541ul, 11030647524ul, 12605512803ul, 9885410786ul,
            11460276065ul, 8740174048ul, 14610006623ul, 11889904606ul,
            13464769885ul, 15039635164ul, 12319533147ul, 9599431130ul,
            11174296409ul, 17044128984ul, 14324026967ul, 3013990358ul,
            8883822933ul, 6163720916ul, 7738586195ul, 9313451474ul,
            10888316753ul, 8168214736ul, 9743080015ul, 11317945294ul,
            8597843277ul, 10172708556ul, 7452606539ul, 13322439114ul,
            10602337097ul, 7882235080ul, 9457100359ul, 11031965638ul,
            12606830917ul, 9886728900ul, 11461594179ul, 8741492162ul,
            10316357441ul, 16186190016ul, 13466087999ul, 10745985982ul,
            12320851261ul, 13895716540ul, 11175614523ul, 8455512506ul,
            14325345081ul, 11605243064ul, 13180108343ul, 10460006326ul,
            12034871605ul, 5019802292ul, 10889634867ul, 12464500146ul,
            9744398129ul, 7024296112ul, 4304194095ul, 10174026670ul,
            11748891949ul, 9028789932ul, 10603655211ul, 12178520490ul,
            9458418473ul, 6738316456ul, 12608149031ul, 9888047014ul,
            11462912293ul, 17332744868ul, 10317675555ul, 7597573538ul,
            9172438817ul, 15042271392ul, 12322169375ul, 9602067358ul,
            11176932637ul, 8456830620ul, 10031695899ul, 11606561178ul,
            13181426457ul, 10461324440ul, 12036189719ul, 13611054998ul,
            10890952981ul, 12465818260ul, 9745716243ul, 11320581522ul,
            12895446801ul, 5880377488ul, 7455242767ul, 9030108046ul,
            10604973325ul, 7884871308ul, 13754703883ul, 6739634570ul,
            8314499849ul, 14184332424ul, 11464230407ul, 8744128390ul,
            10318993669ul, 11893858948ul, 9173756931ul, 6453654914ul,
    };
    static const uint32_t WINDOW_SIZE = 16;
    static const uint32_t CHUNK_CHECKSUM_MASK = ((1u << (uint8_t) (log2(CHUNK_SIZE_AVE - CHUNK_SIZE_MIN))) - 1u);

    // used to calculate the checksum
    uint8_t windows[WINDOW_SIZE]{};
    uint8_t windows_index = 0;
    uint64_t cur_roll_checksum = 0;

    // input data
    ifstream *input_stream;
    uint8_t remain_buffer[CHUNK_SIZE_AVE] = {0};
    uint32_t remain_length;

public:
    explicit chunking(ifstream *input_stream) {
        this->input_stream = input_stream;
        remain_length = 0;
        for (unsigned char &window : windows) {
            window = 0;
        }
    }

    ~chunking() = default;

    /**
     *
     * @param input input data
     * @param input_length the length of the input buffer
     * @param chunks chunk array for holding chunks
     * @return the number of chunk found in this iteration
     */
    uint32_t get_chunks(uint8_t output[], uint32_t chunk_offset[], uint32_t chunk_length[], uint32_t chunk_max) {
        uint32_t curr_chunk_index = 0;
        uint32_t curr_byte_ptr = 0;
        uint32_t curr_byte_max = 0;
        bool read_complete = false;

        if (remain_length > 0) {
            memcpy(output, remain_buffer, remain_length);
            curr_byte_max += remain_length;
            remain_length = 0;
        }

        chunk_offset[0] = 0;
        chunk_length[0] = 0;

        while (curr_chunk_index < chunk_max) {
            // if we can not read byte, then go to exit
            if (input_stream->eof() || input_stream->bad()) {
                if (chunk_length[curr_chunk_index] > 0) {
                    return curr_chunk_index + 1;
                }
                return curr_chunk_index;
            }

            // read in byte till CHUNK_SIZE_AVE
            if (curr_byte_max - curr_byte_ptr < CHUNK_SIZE_AVE) {
                input_stream->read((char *) output + curr_byte_max, CHUNK_SIZE_AVE + curr_byte_ptr - curr_byte_max);
                curr_byte_max += input_stream->gcount();
            }

            while (curr_byte_ptr < curr_byte_max) {
                // if we don't have enough byte, just copy the byte
                if (chunk_length[curr_chunk_index] + curr_byte_max - curr_byte_ptr < CHUNK_JUMP_LEN) {
                    chunk_length[curr_chunk_index] += curr_byte_max - curr_byte_ptr;
                    curr_byte_ptr = curr_byte_max;
                    break;
                }

                // jump the byte until chunk length arrive at CHUNK_JUMP_LEN
                if (chunk_length[curr_chunk_index] < CHUNK_JUMP_LEN) {
                    curr_byte_ptr += CHUNK_JUMP_LEN - chunk_length[curr_chunk_index];
                    chunk_length[curr_chunk_index] = CHUNK_JUMP_LEN;
                }

                while (curr_byte_ptr < curr_byte_max) {
                    uint8_t pushed_out = windows[windows_index];
                    windows[windows_index] = output[curr_byte_ptr];

                    // update windows index
                    windows_index = ((windows_index + 1u) & (15u));

                    // update checksum
                    cur_roll_checksum = (cur_roll_checksum * RAB_POLYNOMIAL_CONST) & POLY_MASK;
                    cur_roll_checksum += output[curr_byte_ptr];
                    cur_roll_checksum -= out[pushed_out];
                    chunk_length[curr_chunk_index]++;
                    curr_byte_ptr++;

                    if (chunk_length[curr_chunk_index] < CHUNK_SIZE_MIN) {
                        continue;
                    }

                    uint64_t cur_pos_checksum = cur_roll_checksum ^ir[pushed_out];
                    if ((cur_pos_checksum & CHUNK_CHECKSUM_MASK) == 0 ||
                        chunk_length[curr_chunk_index] >= CHUNK_SIZE_MAX) {
                        curr_chunk_index++;

                        if (curr_chunk_index == chunk_max) {
                            // copy remain data to local buffer
                            remain_length = curr_byte_max - curr_byte_ptr;
                            memcpy(remain_buffer, output + curr_byte_ptr, remain_length);
                            curr_byte_ptr = curr_byte_max;
                        } else {
                            // begin to find next chunk
                            chunk_offset[curr_chunk_index] = chunk_offset[curr_chunk_index - 1] + chunk_length[curr_chunk_index - 1];
                            chunk_length[curr_chunk_index] = 0;
                        }
                        break;
                    }
                }
            }
        }

        return curr_chunk_index;
    };
};

#endif //ESE532_PROJECT_CHUNKING_H
