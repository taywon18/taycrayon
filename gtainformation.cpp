#include "gtainformation.h"

GtaInformation::GtaInformation()
{
    populate();
}

void GtaInformation::populate()
{
    _motorcycles.insert(463, "Freeway"); //
    _motorcycles.insert(462, "Faggio"); //
    _motorcycles.insert(461, "PCJ 600"); //
    _motorcycles.insert(448, "Pizza boy"); //
    _motorcycles.insert(468, "Sanchez"); //
    _motorcycles.insert(521, "FCR 900"); //
    _motorcycles.insert(522, "NRG 500"); //
    _motorcycles.insert(581, "BF 400"); //
    _motorcycles.insert(586, "Wayfarer"); //

    _bikes.insert(481, "BMX"); //
    _bikes.insert(509, "Bike"); //
    _bikes.insert(510, "Mountain bike"); //

    _airplane.insert(460, "Skimmer"); //
    _airplane.insert(476, "Rustler"); //
    _airplane.insert(511, "Beagle"); //
    _airplane.insert(512, "Cropduster"); //
    _airplane.insert(513, "Stuntplane"); //
    _airplane.insert(519, "Shamal"); //
    _airplane.insert(520, "Hydra"); //
    _airplane.insert(553, "Nevada"); //
    _airplane.insert(577, "AT400"); //
    _airplane.insert(592, "Andromada"); //
    _airplane.insert(593, "Dodo"); //

    _choppa.insert(417, "Leviathan"); //
    _choppa.insert(425, "Hunter"); //
    _choppa.insert(447, "Seasparrow"); //
    _choppa.insert(469, "Sparrow"); //
    _choppa.insert(487, "Maverick"); //
    _choppa.insert(488, "SAN News Maverick"); //
    _choppa.insert(497, "Police Maverick"); //
    _choppa.insert(548, "Cargobob"); //
    _choppa.insert(563, "Raindance"); //

    _boats.insert(430, "Predator"); //
    _boats.insert(446, "Squallo"); //
    _boats.insert(452, "Speeder"); //
    _boats.insert(453, "Reefer"); //
    _boats.insert(454, "Tropic"); //
    _boats.insert(472, "Coastguard"); //
    _boats.insert(473, "Dinghy"); //
    _boats.insert(484, "Marquis"); //
    _boats.insert(493, "Jetmax"); //
    _boats.insert(595, "Launch");

    _land.insert(400, "Landstalker");
    _land.insert(401, "Bravura");
    _land.insert(402, "Buffalo");
    _land.insert(403, "Linerunner");
    _land.insert(404, "Perenniel");
    _land.insert(405, "Sentinel");
    _land.insert(406, "Dumper");
    _land.insert(407, "Firetruck");
    _land.insert(408, "Trashmaster");
    _land.insert(410, "Manana");
    _land.insert(411, "Infernus");
    _land.insert(412, "Voodoo");
    _land.insert(413, "Pony");
    _land.insert(414, "Mule");
    _land.insert(415, "Cheetah");
    _land.insert(416, "Ambulance");
    _land.insert(418, "Moonbeam");
    _land.insert(419, "Esperanto");
    _land.insert(420, "Washington");
    _land.insert(422, "Bobcat");
    _land.insert(424, "BF Injection");
    _land.insert(426, "Premier");
    _land.insert(427, "Enforcer");
    _land.insert(429, "Banshee");
    _land.insert(431, "Bus");
    _land.insert(432, "Rhino");
    _land.insert(433, "Barracks");
    _land.insert(436, "Previon");
    _land.insert(437, "Coach");
    _land.insert(438, "Cabbie");
    _land.insert(439, "Stallion");
    _land.insert(440, "Rumpo");
    _land.insert(443, "Packer");
    _land.insert(444, "Monster");
    _land.insert(445, "Admiral");
    _land.insert(451, "Turismo");
    _land.insert(455, "Flatbed");
    _land.insert(456, "Yankee");
    _land.insert(458, "Solair");
    _land.insert(459, "Topfun Van");
    _land.insert(466, "Glendale");
    _land.insert(467, "Oceanic");
    _land.insert(470, "Patriot");
    _land.insert(474, "Hermes");
    _land.insert(475, "Sabre");
    _land.insert(477, "ZR-350");
    _land.insert(478, "Walton");
    _land.insert(479, "Regina");
    _land.insert(480, "Comet");
    _land.insert(482, "Burrito");
    _land.insert(489, "Rancher");
    _land.insert(490, "FBI Rancher");
    _land.insert(491, "Virgo");
    _land.insert(492, "Greenwood");
    _land.insert(494, "Hotring Racer");
    _land.insert(495, "Sandking");
    _land.insert(496, "Blista Compact");
    _land.insert(498, "Boxville");
    _land.insert(499, "Benson");
    _land.insert(500, "Mesa");
    _land.insert(502, "Hotring Racer A");
    _land.insert(503, "Hotring Racer B");
    _land.insert(504, "Bloodring Banger");
    _land.insert(505, "Rancher Lure");
    _land.insert(506, "Super GT");
    _land.insert(507, "Elegant");
    _land.insert(514, "Tanker");
    _land.insert(515, "Roadtrain");
    _land.insert(516, "Nebula");
    _land.insert(517, "Majestic");
    _land.insert(518, "Buccaneer");
    _land.insert(524, "Cement Truck");
    _land.insert(526, "Fortune");
    _land.insert(527, "Cadrona");
    _land.insert(528, "FBI Truck");
    _land.insert(529, "Willard");
    _land.insert(531, "Tractor");
    _land.insert(533, "Feltzer");
    _land.insert(534, "Remington");
    _land.insert(535, "Slamvan");
    _land.insert(536, "Blade");
    _land.insert(540, "Vincent");
    _land.insert(541, "Bullet");
    _land.insert(542, "Clover");
    _land.insert(543, "Sadler");
    _land.insert(544, "Firetruck LA");
    _land.insert(546, "Intruder");
    _land.insert(547, "Primo");
    _land.insert(549, "Tampa");
    _land.insert(550, "Sunrise");
    _land.insert(551, "Merit");
    _land.insert(552, "Utility Van");
    _land.insert(554, "Yosemite");
    _land.insert(555, "Windsor");
    _land.insert(556, "Monster A");
    _land.insert(557, "Monster B");
    _land.insert(558, "Uranus");
    _land.insert(559, "Jester");
    _land.insert(560, "Sultan");
    _land.insert(561, "Stratum");
    _land.insert(562, "Elegy");
    _land.insert(565, "Flash");
    _land.insert(566, "Tahoma");
    _land.insert(567, "Savanna");
    _land.insert(568, "Bandito");
    _land.insert(573, "Dune");
    _land.insert(575, "Broadway");
    _land.insert(576, "Tornado");
    _land.insert(578, "DFT-30");
    _land.insert(579, "Huntley");
    _land.insert(580, "Stafford");
    _land.insert(582, "Newsvan");
    _land.insert(585, "Emperor");
    _land.insert(586, "Wayfarer");
    _land.insert(587, "Euros");
    _land.insert(596, "Police Car (LSPD)");
    _land.insert(597, "Police Car (SFPD)");
    _land.insert(598, "Police Car (LVPD)");
    _land.insert(599, "Police Ranger");
    _land.insert(600, "Picador");
    _land.insert(601, "S.W.A.T.");
    _land.insert(602, "Alpha");
    _land.insert(603, "Phoenix");
    _land.insert(604, "Glendale Shit");
    _land.insert(605, "Sadler Shit");
    _land.insert(609, "Boxville");

    _land.insert(409, "Stretch");
    _land.insert(423, "Mr. Whoopee");
    _land.insert(428, "Securicar");
    _land.insert(434, "Hotknife");
    _land.insert(442, "Romero");
    _land.insert(457, "Caddy");
    _land.insert(483, "Camper");
    _land.insert(485, "Baggage");
    _land.insert(486, "Dozer");
    _land.insert(508, "Journey");
    _land.insert(525, "Towtruck");
    _land.insert(530, "Forklift");
    _land.insert(532, "Combine Harvester");
    _land.insert(545, "Hustler");
    _land.insert(572, "Mower");
    _land.insert(574, "Sweeper");
    _land.insert(583, "Tug");
    _land.insert(588, "Hotdog");
    _land.insert(539, "Vortex");
}

GtaInformation GtaInformation::Instance()
{
    static GtaInformation info;
    return info;
}

QString GtaInformation::getName(unsigned short id)
{
    if(_motorcycles.contains(id))
        return _motorcycles[id];

    if(_bikes.contains(id))
        return _bikes[id];

    if(_airplane.contains(id))
        return _airplane[id];

    if(_choppa.contains(id))
        return _choppa[id];

    if(_boats.contains(id))
        return _boats[id];

    if(_land.contains(id))
        return _land[id];

    return "";
}
