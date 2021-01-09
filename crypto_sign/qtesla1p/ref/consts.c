/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: constants for the NTT
**************************************************************************************/

#include <stdint.h>
#include "params.h"
#include "poly.h"


const poly zeta ={

152487987, 362708180, 151084668, 393585500, 285507339, 301982154, 215753424, 31155376, 233716852, 465705615, 128512487, 219324660, 50229578, 153034376, 215476824, 300536066, 
335024578, 289187646, 164713195, 268061318, 189759369, 377168836, 116435874, 176113053, 431517196, 190900734, 338220498, 464878555, 173361289, 232250039, 454096337, 436656394, 
195787662, 168565616, 114027909, 101482211, 122304260, 245339257, 270992315, 4286310, 284182633, 281862863, 467931137, 7603533, 87296419, 467325730, 121905925, 68965750, 
7622021, 337743349, 393551614, 47124528, 159423746, 411556895, 272161997, 437233591, 145848369, 293644567, 483816172, 151632492, 404105953, 50166550, 146739314, 412145078, 
15864543, 70750806, 129965017, 341088149, 316683907, 350635104, 130053135, 163148141, 121505175, 321911633, 208679484, 294431751, 453314132, 108343178, 347335911, 164705047, 
162721550, 207020617, 292545544, 470584726, 172724538, 305647837, 149867383, 362637208, 452440748, 260522302, 286994319, 24740874, 202044444, 119470626, 478104674, 80911411, 
414592412, 2221974, 246379318, 129955789, 28799678, 36003616, 152368173, 339611023, 223510693, 291221765, 193132933, 463217793, 331552134, 325636488, 203402489, 295759306, 
248357734, 68229047, 81897305, 240159122, 118053748, 317468757, 422053445, 366729953, 437649988, 255661703, 483132783, 73941682, 473070031, 266941830, 273715645, 293305752, 
97728261, 387416830, 466510576, 359483415, 350097663, 303812937, 61983368, 110765849, 174098312, 288799426, 21108638, 467492225, 174686783, 248408233, 45473451, 384747267, 
405224388, 88952814, 320950556, 7845265, 96262921, 194785009, 345346400, 283984375, 141922183, 197095618, 350922677, 11272999, 89297173, 32109806, 274225692, 315879286, 
447761615, 162801357, 472516440, 273754932, 433704551, 12277378, 129604129, 130765873, 341928374, 97220107, 381055612, 81197393, 281134974, 179347952, 39761035, 34072176, 
156107747, 160521566, 445615052, 382999431, 114761104, 217307868, 361522659, 154828064, 108616610, 483961979, 7594235, 235284403, 243224032, 389934638, 100293270, 300649913, 
252603238, 156952794, 476122165, 413580514, 276718826, 361292064, 361798877, 469444666, 478187612, 366087489, 58364362, 377641792, 464279854, 238407425, 414624898, 95101086, 
321027398, 429694956, 464583265, 437187651, 60881585, 385486792, 312091683, 66834498, 233738788, 348037644, 233604363, 107253653, 131016429, 73947604, 169670982, 290195637, 
119921194, 271135954, 190934119, 290478001, 458387655, 30555018, 243654544, 445824897, 428159318, 284210623, 380070942, 43667456, 346591135, 469657107, 229110312, 89859525, 
279253247, 439931225, 229136222, 321034403, 246424219, 100693825, 214223676, 217294437, 256157960, 295265349, 79251464, 286824915, 454734790, 151846826, 92265815, 248302338, 
324806913, 107169154, 198491413, 107136390, 135693521, 363861623, 129717151, 385117374, 341459776, 364530058, 331840615, 338653108, 179941421, 145194323, 232142444, 310117875, 
245100240, 236651455, 321450096, 408132892, 459643284, 120999848, 331994388, 466033192, 444350281, 46244984, 349366459, 47312667, 210852290, 461907423, 473508756, 243941159, 
334095768, 484392307, 87495054, 79707111, 10325325, 200821060, 83579595, 150963302, 117650353, 241599951, 324099496, 449053341, 36312493, 53120594, 297622605, 80558972, 
378548649, 444726854, 381040843, 435710377, 437798831, 409728224, 337424109, 119488440, 361132895, 56096923, 105509808, 62508901, 220866000, 465933834, 134955292, 165152430, 
461194453, 94958388, 126965781, 461323542, 383847206, 317476902, 163556720, 256017938, 72928042, 371345649, 72741296, 1132229, 282268470, 212115745, 379618538, 106905134, 
239955357, 192105800, 279633365, 261297633, 471571564, 90968104, 459175544, 256276478, 349475049, 94611089, 471946846, 339973147, 179509745, 334215655, 348294589, 246487412, 
338690430, 257410736, 386561992, 130193167, 10375815, 352823358, 66528931, 326800610, 5109748, 350056529, 248785466, 71808467, 91140754, 240803382, 46545554, 148468313, 
217897756, 178220810, 477609665, 208790805, 132277867, 140742422, 254913327, 28667006, 128372915, 79301382, 407186666, 89552874, 246186501, 285455129, 391678316, 275180688, 
128264018, 310322712, 204235493, 392483801, 82342680, 216812163, 464479351, 180360499, 444647478, 203271968, 245690827, 184373912, 109600517, 224566820, 28243984, 271488557, 
21295878, 5023179, 37276402, 156173856, 216282463, 114627758, 400597776, 384792625, 105917732, 345779139, 120948814, 379908150, 352765486, 416168777, 56042296, 349589484, 
144728793, 388255614, 261690821, 484844902, 12918385, 60552278, 302475143, 407425208, 184843522, 76793749, 43262192, 318572606, 192724017, 307990251, 164548393, 289980843, 
119689917, 192248504, 86870040, 113677230, 207612114, 48663937, 453434248, 377339887, 132917063, 336896801, 106411814, 268777744, 109251604, 359574042, 467613823, 362251552, 
400195434, 397227950, 447715984, 156476326, 407995653, 323955098, 143648649, 6572585, 61447842, 60823848, 132376333, 156552612, 447237682, 3352830, 437079412, 293474358, 
288464703, 15328503, 138579659, 221291483, 380584037, 357161755, 205687364, 479414291, 173269786, 295661628, 1098184, 377981714, 433970464, 385767379, 200391723, 463310671, 
470707303, 280545651, 289143978, 1390137, 320221838, 213802405, 52847233, 440579585, 361337171, 467447061, 278099177, 130391690, 112618414, 24013500, 144924227, 296806074, 
251842387, 345992943, 284162718, 360902180, 420467337, 110656360, 350012131, 202820082, 130403434, 353559391, 97646151, 331494212, 207981153, 178821640, 6093504, 174655523, 
25199097, 350851018, 171708038, 460816830, 103136904, 80524000, 275742304, 458552752, 472541551, 190172218, 362093141, 38733526, 9645818, 67304688, 67935446, 108377230, 
311977539, 474309436, 444125532, 103441699, 3468252, 349723698, 97903160, 300076184, 267658035, 238838767, 242177661, 26606481, 135193080, 123327558, 344360834, 367595829, 
356606282, 264791463, 257139528, 162368489, 211890949, 55564884, 438861649, 229441471, 237272101, 233185135, 277657399, 407149255, 91300697, 479685681, 430422637, 119519807, 
421727840, 192635510, 93585631, 135381498, 18652441, 397908973, 188581729, 232327608, 69565403, 391762415, 326530182, 441347692, 230134217, 64984929, 478944650, 351581551, 
190325639, 193670224, 149376075, 211003745, 282313407, 401282565, 142157967, 358980062, 420121767, 23490157, 334750456, 333888004, 267502025, 165819942, 232587558, 86202250, 
302042598, 410443459, 257125477, 225677323, 414705770, 25685059, 301886182, 108044670, 22721383, 270234733, 79875762, 146613845, 419895311, 369960062, 103044731, 31676484, 
286660070, 419993402, 364984450, 18133974, 446533052, 445715499, 406972835, 163019061, 450202465, 241240654, 188881609, 340298195, 253414624, 384263117, 228275432, 255596801, 
10999354, 326104131, 470162945, 345212724, 481147036, 355238387, 445490028, 455415009, 260881193, 427862585, 132654052, 18007865, 59818933, 68142229, 461931681, 213097607, 
330845761, 345671100, 445951421, 379951938, 351197187, 411141392, 7263182, 70699592, 365421264, 376472585, 461484665, 162243510, 31855741, 293625046, 335452299, 133496651, 
449999480, 91675672, 274948339, 231546888, 5898807, 374453904, 415664585, 187816557, 366875998, 16795888, 358324214, 67223590, 208260807, 81751761, 331910299, 126163909, 
197637544, 425142827, 380092754, 432163910, 455820426, 254980283, 256358636, 238549963, 394492520, 207988901, 382035000, 58514991, 406817321, 338951326, 85955701, 385883363, 
199611595, 193846573, 242046917, 445871847, 194675932, 153430125, 126399473, 212399985, 131399120, 150281168, 241101682, 424624124, 463829369, 220399539, 250758923, 286498206, 
389690457, 295269249, 379701698, 289778634, 337591505, 299401947, 408084588, 222627582, 436090373, 268654172, 96674628, 63939283, 63956567, 362372935, 443759942, 75655989, 
451148002, 74813563, 245300859, 92113977, 235932323, 24871709, 445930061, 112311252, 382591591, 341672267, 192081751, 227249338, 104393113, 338268986, 206994950, 319724787, 
213198392, 170478760, 37605185, 158287930, 411833228, 74675173, 290411522, 234950252, 147659191, 477676082, 302215477, 236037294, 396241956, 90037635, 71703941, 189694446, 
127714437, 72581761, 388695298, 324058182, 307942478, 25250803, 430776557, 98441916, 244856801, 54851167, 334050668, 381066122, 91898087, 55879503, 447270860, 473167653, 
219844498, 187193519, 266952433, 391729403, 53226126, 51780224, 371399487, 333787358, 56263300, 280390944, 162069765, 126088217, 254997184, 219544659, 290699753, 91930445, 
262038894, 420981566, 460776098, 376255854, 282871085, 189434060, 447641051, 348987773, 393608679, 229566111, 403901447, 471446519, 411894730, 215726740, 484920069, 409829517, 
306172231, 65880808, 272668213, 300554215, 165356547, 127919123, 432943738, 184462319, 474779995, 312870826, 103228518, 118983222, 470656522, 71417412, 480528917, 63072121, 
276207594, 101611809, 92202812, 400978338, 316767039, 228776701, 96935521, 229917830, 205039402, 397476586, 344074695, 398615665, 107334948, 321477097, 84224896, 289704056, 
337221600, 298175575, 221915312, 193369885, 426129039, 447853316, 54842415, 451086232, 182463979, 451831604, 247440983, 302608727, 173057620, 189163830, 406954853, 263838955, 
390119120, 323062166, 209857219, 233847360, 397630721, 465926004, 10245195, 426687348, 106435173, 177694955, 463447907, 18002511, 30155910, 224512318, 357311179, 265219392, 
299513685, 30291602, 60997329, 10326191, 138035293, 231111502, 414088384, 77026622, 174972782, 4116187, 52092276, 228872080, 15308807, 286405235, 414584533, 228392331, 
399460999, 219125940, 337293712, 187192073, 183965759, 229399789, 51179285, 419512139, 28392143, 424986565, 69672656, 202415584, 216120556, 455470549, 76078626, 278191677, 
228751917, 246802857, 389799018, 342470047, 313959017, 69383448, 340686475, 61470385, 26192776, 409901437, 119717553, 174793260, 123425866, 250300643, 429290586, 387248439, 
183926914, 285901994, 391555377, 468207735, 22116415, 13034202, 399447558, 317569778, 269660832, 44359608, 57347786, 225367797, 253610604, 205790505, 20120029, 285335246, 
351522795, 256987219, 358102627, 167191360, 50532216, 327731504, 369248325, 460123994, 266415358, 194297404, 175124754, 431803266, 172719791, 347394909, 478670700, 198769524, 
172061925, 145181173, 153659180, 261113394, 234615141, 174150073, 188625016, 395363348, 356478592, 350375496, 185431871, 422914531, 144616554, 328948119, 417580486, 132620154, 
208514724, 323125199, 169409944, 209437217, 147304756, 313577692, 467533131, 102365927, 353952516, 47854485, 378899191, 136952166, 46435149, 470355414, 328058320, 292689847, 
24707084, 114822297, 272180086, 182640159, 7628028, 408073454, 377046674, 214221064, 472655903, 91461227, 453378209, 459785635, 126296748, 7369666, 52466342, 269961618, 
231883972, 233439994, 228009066, 436059177, 355741338, 103186287, 391072528, 386169816, 317017329, 449528664, 378844336, 330396197, 137599363, 20911303, 288139426, 164788880, 
266741184, 89618721, 54677577, 457226242, 208474107, 240521207, 158524358, 480614438, 226941889, 385643536, 326188597, 389903564, 264886102, 226747931, 258540220, 407142392, 
};


const poly zetainv ={

227437893, 259230182, 221092011, 96074549, 159789516, 100334577, 259036224, 5363675, 327453755, 245456906, 277504006, 28751871, 431300536, 396359392, 219236929, 321189233, 
197838687, 465066810, 348378750, 155581916, 107133777, 36449449, 168960784, 99808297, 94905585, 382791826, 130236775, 49918936, 257969047, 252538119, 254094141, 216016495, 
433511771, 478608447, 359681365, 26192478, 32599904, 394516886, 13322210, 271757049, 108931439, 77904659, 478350085, 303337954, 213798027, 371155816, 461271029, 193288266, 
157919793, 15622699, 439542964, 349025947, 107078922, 438123628, 132025597, 383612186, 18444982, 172400421, 338673357, 276540896, 316568169, 162852914, 277463389, 353357959, 
68397627, 157029994, 341361559, 63063582, 300546242, 135602617, 129499521, 90614765, 297353097, 311828040, 251362972, 224864719, 332318933, 340796940, 313916188, 287208589, 
7307413, 138583204, 313258322, 54174847, 310853359, 291680709, 219562755, 25854119, 116729788, 158246609, 435445897, 318786753, 127875486, 228990894, 134455318, 200642867, 
465858084, 280187608, 232367509, 260610316, 428630327, 441618505, 216317281, 168408335, 86530555, 472943911, 463861698, 17770378, 94422736, 200076119, 302051199, 98729674, 
56687527, 235677470, 362552247, 311184853, 366260560, 76076676, 459785337, 424507728, 145291638, 416594665, 172019096, 143508066, 96179095, 239175256, 257226196, 207786436, 
409899487, 30507564, 269857557, 283562529, 416305457, 60991548, 457585970, 66465974, 434798828, 256578324, 302012354, 298786040, 148684401, 266852173, 86517114, 257585782, 
71393580, 199572878, 470669306, 257106033, 433885837, 481861926, 311005331, 408951491, 71889729, 254866611, 347942820, 475651922, 424980784, 455686511, 186464428, 220758721, 
128666934, 261465795, 455822203, 467975602, 22530206, 308283158, 379542940, 59290765, 475732918, 20052109, 88347392, 252130753, 276120894, 162915947, 95858993, 222139158, 
79023260, 296814283, 312920493, 183369386, 238537130, 34146509, 303514134, 34891881, 431135698, 38124797, 59849074, 292608228, 264062801, 187802538, 148756513, 196274057, 
401753217, 164501016, 378643165, 87362448, 141903418, 88501527, 280938711, 256060283, 389042592, 257201412, 169211074, 84999775, 393775301, 384366304, 209770519, 422905992, 
5449196, 414560701, 15321591, 366994891, 382749595, 173107287, 11198118, 301515794, 53034375, 358058990, 320621566, 185423898, 213309900, 420097305, 179805882, 76148596, 
1058044, 270251373, 74083383, 14531594, 82076666, 256412002, 92369434, 136990340, 38337062, 296544053, 203107028, 109722259, 25202015, 64996547, 223939219, 394047668, 
195278360, 266433454, 230980929, 359889896, 323908348, 205587169, 429714813, 152190755, 114578626, 434197889, 432751987, 94248710, 219025680, 298784594, 266133615, 12810460, 
38707253, 430098610, 394080026, 104911991, 151927445, 431126946, 241121312, 387536197, 55201556, 460727310, 178035635, 161919931, 97282815, 413396352, 358263676, 296283667, 
414274172, 395940478, 89736157, 249940819, 183762636, 8302031, 338318922, 251027861, 195566591, 411302940, 74144885, 327690183, 448372928, 315499353, 272779721, 166253326, 
278983163, 147709127, 381585000, 258728775, 293896362, 144305846, 103386522, 373666861, 40048052, 461106404, 250045790, 393864136, 240677254, 411164550, 34830111, 410322124, 
42218171, 123605178, 422021546, 422038830, 389303485, 217323941, 49887740, 263350531, 77893525, 186576166, 148386608, 196199479, 106276415, 190708864, 96287656, 199479907, 
235219190, 265578574, 22148744, 61353989, 244876431, 335696945, 354578993, 273578128, 359578640, 332547988, 291302181, 40106266, 243931196, 292131540, 286366518, 100094750, 
400022412, 147026787, 79160792, 427463122, 103943113, 277989212, 91485593, 247428150, 229619477, 230997830, 30157687, 53814203, 105885359, 60835286, 288340569, 359814204, 
154067814, 404226352, 277717306, 418754523, 127653899, 469182225, 119102115, 298161556, 70313528, 111524209, 480079306, 254431225, 211029774, 394302441, 35978633, 352481462, 
150525814, 192353067, 454122372, 323734603, 24493448, 109505528, 120556849, 415278521, 478714931, 74836721, 134780926, 106026175, 40026692, 140307013, 155132352, 272880506, 
24046432, 417835884, 426159180, 467970248, 353324061, 58115528, 225096920, 30563104, 40488085, 130739726, 4831077, 140765389, 15815168, 159873982, 474978759, 230381312, 
257702681, 101714996, 232563489, 145679918, 297096504, 244737459, 35775648, 322959052, 79005278, 40262614, 39445061, 467844139, 120993663, 65984711, 199318043, 454301629, 
382933382, 116018051, 66082802, 339364268, 406102351, 215743380, 463256730, 377933443, 184091931, 460293054, 71272343, 260300790, 228852636, 75534654, 183935515, 399775863, 
253390555, 320158171, 218476088, 152090109, 151227657, 462487956, 65856346, 126998051, 343820146, 84695548, 203664706, 274974368, 336602038, 292307889, 295652474, 134396562, 
7033463, 420993184, 255843896, 44630421, 159447931, 94215698, 416412710, 253650505, 297396384, 88069140, 467325672, 350596615, 392392482, 293342603, 64250273, 366458306, 
55555476, 6292432, 394677416, 78828858, 208320714, 252792978, 248706012, 256536642, 47116464, 430413229, 274087164, 323609624, 228838585, 221186650, 129371831, 118382284, 
141617279, 362650555, 350785033, 459371632, 243800452, 247139346, 218320078, 185901929, 388074953, 136254415, 482509861, 382536414, 41852581, 11668677, 174000574, 377600883, 
418042667, 418673425, 476332295, 447244587, 123884972, 295805895, 13436562, 27425361, 210235809, 405454113, 382841209, 25161283, 314270075, 135127095, 460779016, 311322590, 
479884609, 307156473, 277996960, 154483901, 388331962, 132418722, 355574679, 283158031, 135965982, 375321753, 65510776, 125075933, 201815395, 139985170, 234135726, 189172039, 
341053886, 461964613, 373359699, 355586423, 207878936, 18531052, 124640942, 45398528, 433130880, 272175708, 165756275, 484587976, 196834135, 205432462, 15270810, 22667442, 
285586390, 100210734, 52007649, 107996399, 484879929, 190316485, 312708327, 6563822, 280290749, 128816358, 105394076, 264686630, 347398454, 470649610, 197513410, 192503755, 
48898701, 482625283, 38740431, 329425501, 353601780, 425154265, 424530271, 479405528, 342329464, 162023015, 77982460, 329501787, 38262129, 88750163, 85782679, 123726561, 
18364290, 126404071, 376726509, 217200369, 379566299, 149081312, 353061050, 108638226, 32543865, 437314176, 278365999, 372300883, 399108073, 293729609, 366288196, 195997270, 
321429720, 177987862, 293254096, 167405507, 442715921, 409184364, 301134591, 78552905, 183502970, 425425835, 473059728, 1133211, 224287292, 97722499, 341249320, 136388629, 
429935817, 69809336, 133212627, 106069963, 365029299, 140198974, 380060381, 101185488, 85380337, 371350355, 269695650, 329804257, 448701711, 480954934, 464682235, 214489556, 
457734129, 261411293, 376377596, 301604201, 240287286, 282706145, 41330635, 305617614, 21498762, 269165950, 403635433, 93494312, 281742620, 175655401, 357714095, 210797425, 
94299797, 200522984, 239791612, 396425239, 78791447, 406676731, 357605198, 457311107, 231064786, 345235691, 353700246, 277187308, 8368448, 307757303, 268080357, 337509800, 
439432559, 245174731, 394837359, 414169646, 237192647, 135921584, 480868365, 159177503, 419449182, 133154755, 475602298, 355784946, 99416121, 228567377, 147287683, 239490701, 
137683524, 151762458, 306468368, 146004966, 14031267, 391367024, 136503064, 229701635, 26802569, 395010009, 14406549, 224680480, 206344748, 293872313, 246022756, 379072979, 
106359575, 273862368, 203709643, 484845884, 413236817, 114632464, 413050071, 229960175, 322421393, 168501211, 102130907, 24654571, 359012332, 391019725, 24783660, 320825683, 
351022821, 20044279, 265112113, 423469212, 380468305, 429881190, 124845218, 366489673, 148554004, 76249889, 48179282, 50267736, 104937270, 41251259, 107429464, 405419141, 
188355508, 432857519, 449665620, 36924772, 161878617, 244378162, 368327760, 335014811, 402398518, 285157053, 475652788, 406271002, 398483059, 1585806, 151882345, 242036954, 
12469357, 24070690, 275125823, 438665446, 136611654, 439733129, 41627832, 19944921, 153983725, 364978265, 26334829, 77845221, 164528017, 249326658, 240877873, 175860238, 
253835669, 340783790, 306036692, 147325005, 154137498, 121448055, 144518337, 100860739, 356260962, 122116490, 350284592, 378841723, 287486700, 378808959, 161171200, 237675775, 
393712298, 334131287, 31243323, 199153198, 406726649, 190712764, 229820153, 268683676, 271754437, 385284288, 239553894, 164943710, 256841891, 46046888, 206724866, 396118588, 
256867801, 16321006, 139386978, 442310657, 105907171, 201767490, 57818795, 40153216, 242323569, 455423095, 27590458, 195500112, 295043994, 214842159, 366056919, 195782476, 
316307131, 412030509, 354961684, 378724460, 252373750, 137940469, 252239325, 419143615, 173886430, 100491321, 425096528, 48790462, 21394848, 56283157, 164950715, 390877027, 
71353215, 247570688, 21698259, 108336321, 427613751, 119890624, 7790501, 16533447, 124179236, 124686049, 209259287, 72397599, 9855948, 329025319, 233374875, 185328200, 
385684843, 96043475, 242754081, 250693710, 478383878, 2016134, 377361503, 331150049, 124455454, 268670245, 371217009, 102978682, 40363061, 325456547, 329870366, 451905937, 
446217078, 306630161, 204843139, 404780720, 104922501, 388758006, 144049739, 355212240, 356373984, 473700735, 52273562, 212223181, 13461673, 323176756, 38216498, 170098827, 
211752421, 453868307, 396680940, 474705114, 135055436, 288882495, 344055930, 201993738, 140631713, 291193104, 389715192, 478132848, 165027557, 397025299, 80753725, 101230846, 
440504662, 237569880, 311291330, 18485888, 464869475, 197178687, 311879801, 375212264, 423994745, 182165176, 135880450, 126494698, 19467537, 98561283, 388249852, 192672361, 
212262468, 219036283, 12908082, 412036431, 2845330, 230316410, 48328125, 119248160, 63924668, 168509356, 367924365, 245818991, 404080808, 417749066, 237620379, 190218807, 
282575624, 160341625, 154425979, 22760320, 292845180, 194756348, 262467420, 146367090, 333609940, 449974497, 457178435, 356022324, 239598795, 483756139, 71385701, 405066702, 
7873439, 366507487, 283933669, 461237239, 198983794, 225455811, 33537365, 123340905, 336110730, 180330276, 313253575, 15393387, 193432569, 278957496, 323256563, 321273066, 
138642202, 377634935, 32663981, 191546362, 277298629, 164066480, 364472938, 322829972, 355924978, 135343009, 169294206, 144889964, 356013096, 415227307, 470113570, 73833035, 
339238799, 435811563, 81872160, 334345621, 2161941, 192333546, 340129744, 48744522, 213816116, 74421218, 326554367, 438853585, 92426499, 148234764, 478356092, 417012363, 
364072188, 18652383, 398681694, 478374580, 18046976, 204115250, 201795480, 481691803, 214985798, 240638856, 363673853, 384495902, 371950204, 317412497, 290190451, 49321719, 
31881776, 253728074, 312616824, 21099558, 147757615, 295077379, 54460917, 309865060, 369542239, 108809277, 296218744, 217916795, 321264918, 196790467, 150953535, 185442047, 
270501289, 332943737, 435748535, 266653453, 357465626, 20272498, 252261261, 454822737, 270224689, 183995959, 200470774, 92392613, 334893445, 123269933, 333490126, 78835721, 
};

