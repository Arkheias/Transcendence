//	Utilities.cpp
//
//	Utility classes

#include "PreComp.h"
#include <math.h>

#define CLASS_ATTRIB						CONSTLIT("class")
#define CUSTOM_PLURAL_ATTRIB				CONSTLIT("customPlural")
#define DAMAGE_ADJ_ATTRIB					CONSTLIT("damageAdj")
#define DEFINITE_ARTICLE_ATTRIB				CONSTLIT("definiteArticle")
#define ES_PLURAL_ATTRIB					CONSTLIT("esPlural")
#define FIRST_PLURAL_ATTRIB					CONSTLIT("firstPlural")
#define HP_BONUS_ATTRIB						CONSTLIT("hpBonus")
#define NO_ARTICLE_ATTRIB					CONSTLIT("noArticle")
#define PERSONAL_NAME_ATTRIB				CONSTLIT("personalName")
#define SCALE_ATTRIB						CONSTLIT("scale")
#define SECOND_PLURAL_ATTRIB				CONSTLIT("secondPlural")
#define SOVEREIGN_ATTRIB					CONSTLIT("sovereign")
#define VOWEL_ARTICLE_ATTRIB				CONSTLIT("reverseArticle")

#define FREQUENCY_COMMON					CONSTLIT("common")
#define FREQUENCY_UNCOMMON					CONSTLIT("uncommon")
#define FREQUENCY_RARE						CONSTLIT("rare")
#define FREQUENCY_VERYRARE					CONSTLIT("veryrare")
#define FREQUENCY_NOTRANDOM					CONSTLIT("notrandom")

#define ARMOR_COMPOSITION_METALLIC			CONSTLIT("metallic")
#define ARMOR_COMPOSITION_CERAMIC			CONSTLIT("ceramic")
#define ARMOR_COMPOSITION_CARBIDE			CONSTLIT("carbide")
#define ARMOR_COMPOSITION_NANOSCALE			CONSTLIT("nanoscale")
#define ARMOR_COMPOSITION_QUANTUM			CONSTLIT("quantum")
#define ARMOR_COMPOSITION_GRAVITIC			CONSTLIT("gravitic")
#define ARMOR_COMPOSITION_DARKMATTER		CONSTLIT("darkmatter")

#define VERSION_095							(CONSTLIT("0.95"))
#define VERSION_095A						(CONSTLIT("0.95a"))
#define VERSION_095B						(CONSTLIT("0.95b"))
#define VERSION_096							(CONSTLIT("0.96"))
#define VERSION_096A						(CONSTLIT("0.96a"))
#define VERSION_097							(CONSTLIT("0.97"))
#define VERSION_097A						(CONSTLIT("0.97a"))
#define VERSION_098							(CONSTLIT("0.98"))
#define VERSION_098A						(CONSTLIT("0.98a"))
#define VERSION_098B						(CONSTLIT("0.98b"))
#define VERSION_098C						(CONSTLIT("0.98c"))
#define VERSION_098D						(CONSTLIT("0.98d"))
#define VERSION_099							(CONSTLIT("0.99"))
#define VERSION_099A						(CONSTLIT("0.99a"))
#define VERSION_099B						(CONSTLIT("0.99b"))
#define VERSION_099C						(CONSTLIT("0.99c"))
#define VERSION_100							(CONSTLIT("1.0"))
#define VERSION_110							(CONSTLIT("1.1"))

#define CONTROLLER_FLEET					CONSTLIT("fleet")
#define CONTROLLER_FERIAN					CONSTLIT("ferian")
#define CONTROLLER_AUTON					CONSTLIT("auton")
#define CONTROLLER_GLADIATOR				CONSTLIT("gladiator")
#define CONTROLLER_FLEET_COMMAND			CONSTLIT("fleetcommand")
#define CONTROLLER_GAIAN_PROCESSOR			CONSTLIT("gaianprocessor")
#define CONTROLLER_ZOANTHROPE				CONSTLIT("zoanthrope")

#define INTER_SYSTEM_FOLLOW_PLAYER			CONSTLIT("followPlayer")
#define INTER_SYSTEM_WAIT_FOR_PLAYER		CONSTLIT("waitForPlayer")

#define SCALE_AU							CONSTLIT("AU")
#define SCALE_LIGHT_MINUTE					CONSTLIT("light-minute")
#define SCALE_LIGHT_SECOND					CONSTLIT("light-second")
#define SCALE_PIXEL							CONSTLIT("pixel")

static Metric g_RangeIndex[RANGE_INDEX_COUNT] =
	{
	(500.0 * LIGHT_SECOND),
	(340.0 * LIGHT_SECOND),
	(225.0 * LIGHT_SECOND),
	(150.0 * LIGHT_SECOND),
	(100.0 * LIGHT_SECOND),
	(50.0 * LIGHT_SECOND),
	(25.0 * LIGHT_SECOND),
	(13.0 * LIGHT_SECOND),
	(6.0 * LIGHT_SECOND),
	};

struct SOrderTypeData
	{
	char *szName;
	char *szTarget;
	//	-		no target
	//	*		optional target
	//	o		required target

	char *szData;
	//	-		no data
	//	i		integer (may be optional)
	//	2		two integers (encoded in a DWORD)
	};

static const SOrderTypeData g_OrderTypes[] =
	{
		//	name					target	data
		{	"",						"-",	"-"	 },

		{	"guard",				"o",	"-" },
		{	"dock",					"o",	"-" },
		{	"attack",				"o",	"i" },
		{	"wait",					"-",	"i" },
		{	"gate",					"*",	"-" },

		{	"gateOnThreat",			"-",	"-" },
		{	"gateOnStationDestroyed",	"-",	"-" },
		{	"patrol",				"o",	"i" },
		{	"escort",				"o",	"2"	},
		{	"scavenge",				"-",	"-" },

		{	"followPlayerThroughGate",	"o",	"-" },
		{	"attackNearestEnemy",	"-",	"-" },
		{	"tradeRoute",			"-",	"-" },
		{	"wander",				"-",	"-" },
		{	"loot",					"o",	"-" },

		{	"hold",					"-",	"i" },
		{	"mine",					"o",	"-" },
		{	"waitForPlayer",		"-",	"-" },
		{	"attackPlayerOnReturn",	"-",	"-" },
		{	"follow",				"o",	"-" },

		{	"navPath",				"-",	"i"	},
		{	"goto",					"o",	"-" },
		{	"waitForTarget",		"o",	"i" },
		{	"waitForEnemy",			"-",	"i" },
		{	"bombard",				"o",	"i" },

		{	"approach",				"o",	"i" },
		{	"aim",					"o",	"-" },
		{	"orbit",				"o",	"2" },
		{	"holdCourse",			"-",	"2" },
		{	"turnTo",				"-",	"i" },

		{	"attackHold",			"o",	"i" },
		{	"attackStation",		"o",	"i" },
	};

#define ORDER_TYPES_COUNT		(sizeof(g_OrderTypes) / sizeof(g_OrderTypes[0]))

static char *g_pszDestructionCauses[] =
	{
	"",

	"damage",
	"runningOutOfFuel",
	"radiationPoisoning",
	"self",
	"disintegration",

	"weaponMalfunction",
	"ejecta",
	"explosion",
	"shatter",
	"playerCreatedExplosion",

	"enteredStargate",
	"custom",
	};

#define DESTRUCTION_CAUSES_COUNT	(sizeof(g_pszDestructionCauses) / sizeof(g_pszDestructionCauses[0]))

static char *g_pszDamageResults[] =
	{
	"noDamage",
	"absorbedByShields",
	"armorHit",
	"structuralHit",
	"destroyed",
	"passthrough",
	"passthroughDestroyed",
	"destroyedAbandoned",
	};

#define DAMAGE_RESULTS_COUNT		(sizeof(g_pszDamageResults) / sizeof(g_pszDamageResults[0]))

static char *g_pszMessageID[] =
	{
	"",							//	0
	"AttackTarget",				//	msgAttack
	"AttackTargetBroadcast",	//	msgDestroyBroadcast
	"HitByFriendlyFire",		//	msgHitByFriendlyFire
	"QueryEscortStatus",		//	msgQueryEscortStatus
	"QueryFleetStatus",			//	msgQueryFleetStatus
	"EscortAttacked",			//	msgEscortAttacked
	"EscortReportingIn",		//	msgEscortReportingIn
	"WatchYourTargets",			//	msgWatchTargets
	"NiceShooting",				//	msgNiceShooting
	"FormUp",					//	msgFormUp
	"BreakAndAttack",			//	msgBreakAndAttack
	"QueryComms",				//	msgQueryCommunications
	"AbortAttack",				//	msgAbortAttack
	"Wait",						//	msgWait
	"QueryWaitStatus",			//	msgQueryWaitStatus
	"AttackInFormation",		//	msgAttackInFormation
	"DeterTarget",				//	msgAttackDeter
	"QueryAttackStatus",		//	msgQueryAttackStatus
	};

#define MESSAGE_ID_COUNT			(sizeof(g_pszMessageID) / sizeof(g_pszMessageID[0]))

struct SGenomeData
	{
	char *pszID;
	char *pszName;
	};

static SGenomeData g_Genome[] =
	{
		{	"",		""	},						//	genomeUnknown
		{	"humanMale",	"human male"	},	//	genomeMale
		{	"humanFemale",	"human female"	},	//	genomeFemale
	};

#define GENOME_COUNT				(sizeof(g_Genome) / sizeof(g_Genome[0]))

static char *TITLE_CAP_EXCEPTIONS[] =
	{
	"a",
	"an",
	"and",
	"at",
	"by",
	"for",
	"in",
	"near",
	"not",
	"of",
	"on",
	"or",
	"rce",		//	(abbr.) Registerd Corporate Entity 
	"the",
	"to",
	"under",
	"upon",
	};

static int TITLE_CAP_EXCEPTIONS_COUNT = sizeof(TITLE_CAP_EXCEPTIONS) / sizeof(TITLE_CAP_EXCEPTIONS[0]);

static Metric g_pDestinyToBellCurve[g_DestinyRange];
static bool g_bDestinyToBellCurveInit = false;

//	CDiceRange ----------------------------------------------------------------

DiceRange::DiceRange (int iFaces, int iCount, int iBonus) :
		m_iFaces(iFaces),
		m_iCount(iCount),
		m_iBonus(iBonus)

//	CDiceRange constructor

	{
	}

ALERROR DiceRange::LoadFromXML (const CString &sAttrib, int iDefault, CString *retsSuffix)

//	LoadFromXML
//
//	Loads from an XML attribute

	{
	bool bNullValue;
	char *pPos = sAttrib.GetASCIIZPointer();

	//	If empty, then default to 0

	if (*pPos == '\0')
		{
		m_iCount = 0;
		m_iFaces = 0;
		m_iBonus = iDefault;
		if (retsSuffix)
			*retsSuffix = NULL_STR;
		return NOERROR;
		}

	//	First is the number of dice

	int iCount = strParseInt(pPos, 0, &pPos, &bNullValue);
	if (bNullValue)
		return ERR_FAIL;

	//	If we've got a 'd' then we have a dice pattern

	if (*pPos == 'd')
		{
		pPos++;
		m_iCount = iCount;
		if (iCount < 0)
			return ERR_FAIL;

		//	Now parse the sides

		m_iFaces = strParseInt(pPos, -1, &pPos, NULL);
		if (m_iFaces == -1)
			return ERR_FAIL;

		//	Finally, add any bonus

		if (*pPos != '\0')
			m_iBonus = strParseInt(pPos, 0, &pPos, NULL);
		else
			m_iBonus = 0;
		}

	//	If we've got a '-' then we have a range pattern

	else if (*pPos == '-')
		{
		pPos++;
		int iEnd = strParseInt(pPos, 0, &pPos, &bNullValue);
		if (bNullValue)
			return ERR_FAIL;

		if (iEnd < iCount)
			Swap(iEnd, iCount);

		m_iCount = 1;
		m_iFaces = (iEnd - iCount) + 1;
		m_iBonus = iCount - 1;
		}

	//	Otherwise, we've got a constant number

	else
		{
		m_iCount = 0;
		m_iFaces = 0;
		m_iBonus = iCount;
		}

	//	Check trailing suffix

	if (retsSuffix)
		{
		//	Skip whitespace

		while (::strIsWhitespace(pPos))
			pPos++;

		char *pStart = pPos;

		//	Skip whitespace at the end

		while (*pPos != '\0' && !::strIsWhitespace(pPos))
			pPos++;

		//	Done

		*retsSuffix = CString(pStart, pPos - pStart);
		}

	return NOERROR;
	}

void DiceRange::ReadFromStream (SLoadCtx &Ctx)

//	ReadFromStream
//
//	Read the range

	{
	if (Ctx.dwVersion >= 27)
		{
		Ctx.pStream->Read((char *)&m_iFaces, sizeof(DWORD));
		Ctx.pStream->Read((char *)&m_iCount, sizeof(DWORD));
		Ctx.pStream->Read((char *)&m_iBonus, sizeof(DWORD));
		}
	else
		{
		DWORD dwLoad;
		Ctx.pStream->Read((char *)&dwLoad, sizeof(DWORD));

		m_iFaces = HIBYTE(LOWORD(dwLoad));
		m_iCount = LOBYTE(LOWORD(dwLoad));
		m_iBonus = (int)(short)HIWORD(dwLoad);
		}
	}

int DiceRange::Roll (void) const

//	Roll
//
//	Generate a random number

	{
	int iRoll = 0;

	for (int i = 0; i < m_iCount; i++)
		iRoll += mathRandom(1, m_iFaces);

	return iRoll + m_iBonus;
	}

CString DiceRange::SaveToXML (void) const

//	SaveToXML
//
//	Represents range as an attribute value

	{
	if (m_iCount > 0 && m_iFaces > 0)
		{
		if (m_iBonus > 0)
			return strPatternSubst("%dd%d+%d", m_iCount, m_iFaces, m_iBonus);
		else if (m_iBonus < 0)
			return strPatternSubst("%dd%d-%d", m_iCount, m_iFaces, -m_iBonus);
		else
			return strPatternSubst("%dd%d", m_iCount, m_iFaces);
		}
	else
		return strFromInt(m_iBonus, TRUE);
	}

void DiceRange::Scale (Metric rScale)

//	Scale
//
//	Adjust values by given factor

	{
	if (m_iCount == 0)
		m_iBonus = (int)(m_iBonus * rScale + 0.5);
	else if (m_iCount == 1)
		{
		Metric rNewMid = (m_iBonus + (m_iFaces + 1) / 2.0) * rScale;

		m_iFaces = (int)(m_iFaces * rScale + 0.5);
		m_iBonus = (int)((rNewMid - (m_iFaces + 1) / 2.0) + 0.5);
		}
	else
		{
		m_iCount = (int)(m_iCount * rScale + 0.5);
		m_iBonus = (int)(m_iBonus * rScale + 0.5);
		}
	}

void DiceRange::WriteToStream (IWriteStream *pStream) const

//	WriteToStream
//
//	Writes the range

	{
	pStream->Write((char *)&m_iFaces, sizeof(DWORD));
	pStream->Write((char *)&m_iCount, sizeof(DWORD));
	pStream->Write((char *)&m_iBonus, sizeof(DWORD));
	}

//	Miscellaneous functions

CString AppendModifiers (const CString &sModifierList1, const CString &sModifierList2)

//	AppendModifiers
//
//	Returns a concatenation of the two modifier lists

	{
	if (sModifierList1.IsBlank())
		return sModifierList2;
	else if (sModifierList2.IsBlank())
		return sModifierList1;
	else
		{
		CString sList1 = strTrimWhitespace(sModifierList1);
		if (sList1.IsBlank())
			return sModifierList2;

		CString sList2 = strTrimWhitespace(sModifierList2);
		if (sList2.IsBlank())
			return sModifierList1;

		char *pLastChar = (sList1.GetASCIIZPointer() + sList1.GetLength() - 1);
		if (*pLastChar != ',' && *pLastChar != ';')
			sList1.Append(CONSTLIT(", "));

		sList1.Append(sList2);

		return sList1;
		}
	}

void AppendReferenceString (CString *iosReference, const CString &sString)
	{
	if (sString.IsBlank())
		NULL;
	else if (iosReference->IsBlank())
		*iosReference = sString;
	else
		{
		iosReference->Append(CONSTLIT(" � "));
		iosReference->Append(sString);
		}
	}

int CalcEffectiveHP (int iLevel, int iHP, int *iHPbyDamageType)

//	CalcEffectiveHP
//
//	Calculates the effective HP for the armor/shields, taking into account damage type adj

	{
	int i;
	const int WEIGHT_COUNT = 5;
	const Metric WEIGHT_DIST[WEIGHT_COUNT] = { 0.84, 0.65, 0.42, 0.21, 0.08 };

	//	Add all the values together, adjusting for how common a given
	//	damage type is at a particular level

	Metric rTotalHP = 0.0;
	Metric rTotalWeight = 0.0;
	for (i = 0; i < damageCount; i++)
		{
		int iMidLevel = GetDamageTypeLevel((DamageTypes)i) + 1;
		int iDiff = Absolute(iMidLevel - iLevel);
		int iHPatLevel = iHPbyDamageType[i];
		if (iHPatLevel == -1)
			iHPatLevel = iHP * 10;

		if (iDiff < WEIGHT_COUNT)
			{
			rTotalHP += iHPatLevel * WEIGHT_DIST[iDiff];
			rTotalWeight += WEIGHT_DIST[iDiff];
			}
		}

	//	Done

	if (rTotalWeight > 0.0)
		return (int)((rTotalHP / rTotalWeight) + 0.5);
	else
		return 0;
	}

IShipController::ManeuverTypes CalcTurnManeuver (int iDesired, int iCurrent, int iRotationAngle)

//	CalcTurnManeuver
//
//	Calculates the turn maneuver required to face the given direction

	{
	int iTurn = (iDesired + 360 - iCurrent) % 360;

	if ((iTurn >= (360 - (iRotationAngle / 2)))
			|| (iTurn <= (iRotationAngle / 2)))
		return IShipController::NoRotation;
	else
		{
		if (iTurn >= 180)
			return IShipController::RotateRight;
		else
			return IShipController::RotateLeft;
		}
	}

CString ComposeDamageAdjReference (int *AdjRow, int *StdRow)

//	ComposeDamageAdjReference
//
//	Compose a string that describes the damage adjustments

	{
	CString sResult;
	CString sVulnerable;
	CString sResistant;
	CString sImmune;
	int iVulnerableCount = 0;
	int iResistantCount = 0;
	int iImmuneCount = 0;
	int iDamage;
	bool bInvertVulnerable = false;
	bool bInvertResistant = false;
	bool bInvertImmune = false;

	//	Compute immunities and vulnerabilities

	for (iDamage = 0; iDamage < damageCount; iDamage++)
		{
		int iAdj = AdjRow[iDamage];
		int iStd = StdRow[iDamage];

		if (iAdj != iStd)
			{
			//	Append

			if (iAdj > iStd)
				{
				if (sVulnerable.IsBlank())
					sVulnerable = strPatternSubst(CONSTLIT("%s"), GetDamageShortName((DamageTypes)iDamage));
				else
					sVulnerable.Append(strPatternSubst(CONSTLIT(", %s"), GetDamageShortName((DamageTypes)iDamage)));

				iVulnerableCount++;
				}
			else if (iAdj < 20)
				{
				if (sImmune.IsBlank())
					sImmune = strPatternSubst(CONSTLIT("%s"), GetDamageShortName((DamageTypes)iDamage));
				else
					sImmune.Append(strPatternSubst(CONSTLIT(", %s"), GetDamageShortName((DamageTypes)iDamage)));

				iImmuneCount++;
				}
			else
				{
				if (sResistant.IsBlank())
					sResistant = strPatternSubst(CONSTLIT("%s"), GetDamageShortName((DamageTypes)iDamage));
				else
					sResistant.Append(strPatternSubst(CONSTLIT(", %s"), GetDamageShortName((DamageTypes)iDamage)));

				iResistantCount++;
				}
			}
		}

	//	If we've got a lot of immunities, then reverse the sense 
	//	"immune to all except:..."

	if (iImmuneCount > (damageCount - 3) && iVulnerableCount == 0 && iResistantCount == 0)
		{
		sImmune = NULL_STR;
		bInvertImmune = true;

		for (iDamage = 0; iDamage < damageCount; iDamage++)
			{
			int iAdj = AdjRow[iDamage];
			int iStd = StdRow[iDamage];

			if (iAdj == iStd)
				{
				if (sImmune.IsBlank())
					sImmune = strPatternSubst(CONSTLIT("%s"), GetDamageShortName((DamageTypes)iDamage));
				else
					sImmune.Append(strPatternSubst(CONSTLIT(", %s"), GetDamageShortName((DamageTypes)iDamage)));
				}
			}
		}

	//	If we've got a lot of resistance, then reverse the sense 
	//	"resistant to all except:..."

	if (iResistantCount > (damageCount - 3) && iImmuneCount == 0 && iVulnerableCount == 0)
		{
		sResistant = NULL_STR;
		bInvertResistant = true;

		for (iDamage = 0; iDamage < damageCount; iDamage++)
			{
			int iAdj = AdjRow[iDamage];
			int iStd = StdRow[iDamage];

			if (iAdj == iStd)
				{
				if (sResistant.IsBlank())
					sResistant = strPatternSubst(CONSTLIT("%s"), GetDamageShortName((DamageTypes)iDamage));
				else
					sResistant.Append(strPatternSubst(CONSTLIT(", %s"), GetDamageShortName((DamageTypes)iDamage)));
				}
			}
		}

	//	If we've got a lot of vulnerabilities, then reverse the sense 
	//	"vulnerable to all except:..."

	if (iVulnerableCount > (damageCount - 3) && iImmuneCount == 0 && iResistantCount == 0)
		{
		sVulnerable = NULL_STR;
		bInvertVulnerable = true;

		for (iDamage = 0; iDamage < damageCount; iDamage++)
			{
			int iAdj = AdjRow[iDamage];
			int iStd = StdRow[iDamage];

			if (iAdj == iStd)
				{
				if (sVulnerable.IsBlank())
					sVulnerable = strPatternSubst(CONSTLIT("%s"), GetDamageShortName((DamageTypes)iDamage));
				else
					sVulnerable.Append(strPatternSubst(CONSTLIT(", %s"), GetDamageShortName((DamageTypes)iDamage)));
				}
			}
		}

	//	Normal immunities

	if (!sImmune.IsBlank())
		{
		if (iImmuneCount == damageCount)
			sResult.Append(CONSTLIT("; immune to all damage"));
		else if (iImmuneCount == 1)
			sResult.Append(strPatternSubst(CONSTLIT("; %s-immune"), sImmune));
		else if (bInvertImmune)
			sResult.Append(strPatternSubst(CONSTLIT("; immune to all except: %s"), sImmune));
		else
			sResult.Append(strPatternSubst(CONSTLIT("; immune to: %s"), sImmune));
		}

	if (!sResistant.IsBlank())
		{
		if (iResistantCount == damageCount)
			sResult.Append(CONSTLIT("; resistant to all damage"));
		else if (iResistantCount == 1)
			sResult.Append(strPatternSubst(CONSTLIT("; %s-resistant"), sResistant));
		else if (iResistantCount + iImmuneCount == damageCount)
			sResult.Append(CONSTLIT("; resistant to all other damage"));
		else if (bInvertResistant)
			sResult.Append(strPatternSubst(CONSTLIT("; resistant to all except: %s"), sResistant));
		else
			sResult.Append(strPatternSubst(CONSTLIT("; resistant to: %s"), sResistant));
		}

	if (!sVulnerable.IsBlank())
		{
		if (iVulnerableCount == damageCount)
			sResult.Append(CONSTLIT("; vulnerable to all damage"));
		else if (iVulnerableCount == 1)
			sResult.Append(strPatternSubst(CONSTLIT("; %s-vulnerable"), sVulnerable));
		else if (iVulnerableCount + iResistantCount + iImmuneCount == damageCount)
			sResult.Append(CONSTLIT("; vulnerable to all other damage"));
		else if (bInvertVulnerable)
			sResult.Append(strPatternSubst(CONSTLIT("; vulnerable to all except: %s"), sVulnerable));
		else
			sResult.Append(strPatternSubst(CONSTLIT("; vulnerable to: %s"), sVulnerable));
		}

	return sResult;
	}

CString ComposeNounPhrase (const CString &sNoun, 
						   int iCount, 
						   const CString &sModifier,
						   DWORD dwNounFlags, 
						   DWORD dwComposeFlags)

//	ComposeNounPhrase
//
//	Composes a noun phrase based on the appropriate flags

	{
	//	Figure out whether we need to pluralize or not

	bool bPluralize = (dwComposeFlags & nounPlural)
			|| (iCount > 1 
				&& ((dwComposeFlags & nounCount) || (dwComposeFlags & nounCountOnly) || (dwComposeFlags & nounDemonstrative))
				&& !(dwComposeFlags & nounArticle));

	//	Get the proper noun form

	CString sNounForm = ParseNounForm(sNoun, dwNounFlags, bPluralize, (dwComposeFlags & nounShort) != 0);

	//	Get the appropriate article

	CString sArticle;
	if ((dwComposeFlags & nounArticle)
			|| ((dwComposeFlags & nounCount) && iCount == 1))
		{
		if (dwNounFlags & nounNoArticle)
			sArticle = NULL_STR;
		else if (dwNounFlags & nounDefiniteArticle)
			sArticle = CONSTLIT("the ");
		else
			{
			char *pFirstLetter;
			if (sModifier.IsBlank())
				pFirstLetter = sNounForm.GetPointer();
			else
				pFirstLetter = sModifier.GetPointer();

			switch (*pFirstLetter)
				{
				case 'A':
				case 'a':
				case 'E':
				case 'e':
				case 'I':
				case 'i':
				case 'O':
				case 'o':
				case 'U':
				case 'u':
					{
					if (dwNounFlags & nounVowelArticle)
						sArticle = CONSTLIT("a ");
					else
						sArticle = CONSTLIT("an ");
					break;
					}

				default:
					{
					if (dwNounFlags & nounVowelArticle)
						sArticle = CONSTLIT("an ");
					else
						sArticle = CONSTLIT("a ");
					}
				}
			}
		}
	else if (dwComposeFlags & nounDemonstrative)
		{
		if (dwNounFlags & nounNoArticle)
			sArticle = NULL_STR;
		else if (dwNounFlags & nounDefiniteArticle)
			sArticle = CONSTLIT("the ");
		else
			{
			if (iCount > 1)
				sArticle = CONSTLIT("these ");
			else
				sArticle = CONSTLIT("this ");
			}
		}
	else if (iCount > 1
			&& ((dwComposeFlags & nounCount) || (dwComposeFlags & nounCountOnly)))
		{
		sArticle = strFromInt(iCount, false);
		sArticle.Append(CONSTLIT(" "));
		}

	//	Compose

	CString sNounPhrase = strPatternSubst(CONSTLIT("%s%s%s"), 
				sArticle, 
				sModifier, 
				sNounForm);

	if (dwComposeFlags & nounTitleCapitalize)
		return strTitleCapitalize(sNounPhrase, TITLE_CAP_EXCEPTIONS, TITLE_CAP_EXCEPTIONS_COUNT);
	else if (dwComposeFlags & nounCapitalize)
		return strCapitalize(sNounPhrase);
	else
		return sNounPhrase;
	}

void ComputePercentages (int iCount, int *pTable)

//	ComputePercentages
//
//	Given an array of integers representing weights, this function
//	modifies the array to contain a percentages proportional to the
//	weights.

	{
	struct TableEntry
		{
		int iChance;
		int iRemainder;
		};

	int i;
	TableEntry *pScratch = new TableEntry[iCount];

	//	Add up the total weights of all items

	int iTotalScore = 0;
	for (i = 0; i < iCount; i++)
		iTotalScore += pTable[i];

	//	Done if no scores

	if (iTotalScore <= 0)
		{
		delete [] pScratch;
		return;
		}

	//	Compute the chance

	int iTotalChance = 0;
	for (i = 0; i < iCount; i++)
		{
		pScratch[i].iChance = (pTable[i] * 100) / iTotalScore;
		pScratch[i].iRemainder = (pTable[i] * 100) % iTotalScore;

		iTotalChance += pScratch[i].iChance;
		}

	//	Distribute the remaining chance points

	while (iTotalChance < 100)
		{
		//	Look for the entry with the biggest remainder

		int iBestRemainder = 0;
		int iBestEntry = -1;

		for (i = 0; i < iCount; i++)
			if (pScratch[i].iRemainder > iBestRemainder)
				{
				iBestRemainder = pScratch[i].iRemainder;
				iBestEntry = i;
				}

		pScratch[iBestEntry].iChance++;
		pScratch[iBestEntry].iRemainder = 0;
		iTotalChance++;
		}

	//	Modify original

	for (i = 0; i < iCount; i++)
		pTable[i] = pScratch[i].iChance;

	//	Done

	delete [] pScratch;
	}

#if 0
CVector ConvertObjectPos2Pos (int iAngle, Metric rRadius, Metric rHeight, Metric rImageSize)

//	ConvertObjectPos2Pos
//
//	Convert from a position on a rendered 3D object to a position relative
//	to the center of the object's 2D image.
//
//	rRadius and rHeight are in units, 0 degrees is straight up.
//	rImageSize is the size of the rendered image in pixels.
//
//	Note: We expect rHeight and rRadius to be less than 12.0

	{
	const Metric CAMERA_Z = 12.0;
	const Metric CAMERA_Y = 6.0;
	const Metric CAMERA_DIST = 13.4164;		//	Distance from origin to camera
	const Metric CAMERA_SIN = 0.447214;		//	Sin Theta (angle of camera plane wrt object plane)
	const Metric CAMERA_FIELD = 6.0;		//	Camera field of view in units

	//	If no image size, then assume a simple rotation

	if (rImageSize == 0.0)
		return PolarToVector(iAngle, rRadius * g_KlicksPerPixel);

	//	Convert the object pos to cartessian

	CVector vPos = PolarToVector(iAngle, rRadius);

	//	Adjust the y axis for height

	vPos.SetY(vPos.GetY() + ((rHeight * (vPos.GetY() + CAMERA_Y)) / (CAMERA_Z - rHeight)));

	//	Adjust the y axis for camera projection

	vPos.SetY((CAMERA_DIST * vPos.GetY()) / (CAMERA_DIST + (CAMERA_SIN * vPos.GetY())));

	//	Convert to klicks

	return vPos * (g_KlicksPerPixel * rImageSize / CAMERA_FIELD);
	}
#endif

int ComputeWeightAdjFromMatchStrength (bool bHasAttrib, int iMatchStrength)

//	ComputeWeightAdjFromMatchStrength
//
//	If iMatchStrength is positive, then it means we want a certain
//	attribute. If we have the attribute, then we increase our probability
//	but if we don't have the attribute, our probability is normal.
//
//	If iMatchStrength is negative, then it means we DO NOT want a
//	certain attribute. If we have the attribute, then decrease our
//	probability. Otherwise, our probability is normal.
//
//	OPTION	CODE		HAVE ATTRIB			DON'T HAVE ATTRIB
//	---------------------------------------------------------
//	!		-4			x 0					x 1
//	---		-3			x 1/20				x 1
//	--		-2			x 1/5				x 1
//	-		-1			x 1/2				x 1
//	+		+1			x 2					x 1
//	++		+2			x 5					x 1
//	+++		+3			x 5					x 1/2
//	*		+4			x 5					x 0

	{
	switch (iMatchStrength)
		{
		case -4:
			return (bHasAttrib ? 0 : 1000);

		case -3:
			return (bHasAttrib ? 50 : 1000);

		case -2:
			return (bHasAttrib ? 200 : 1000);

		case -1:
			return (bHasAttrib ? 500 : 1000);

		case 1:
			return (bHasAttrib ? 2000 : 1000);

		case 2:
			return (bHasAttrib ? 5000 : 1000);

		case 3:
			return (bHasAttrib ? 5000 : 500);

		case 4:
			return (bHasAttrib ? 5000 : 0);

		default:
			return 1000;
		}
	}

IShipController *CreateShipController (const CString &sAI)

//	CreateShipController
//
//	Creates the appropriate controller
//
//	The following controllers are valid:
//
//	""				The standard AI
//	"auton"			Auton AI
//	"ferian"		Ferian ship AI
//	"fleet"			The fleet controller for formation flying
//	"fleetcommand"	Controller for commanding a fleet
//	"gaianprocessor"Controller for Gaian processor
//	"gladiator"		DEPRECATED
//	"zoanthrope"	Zoanthrope AI

	{
	if (sAI.IsBlank())
		return new CStandardShipAI;
	else if (strEquals(sAI, CONTROLLER_FLEET))
		return new CFleetShipAI;
	else if (strEquals(sAI, CONTROLLER_FERIAN))
		return new CFerianShipAI;
	else if (strEquals(sAI, CONTROLLER_AUTON))
		return new CAutonAI;
	else if (strEquals(sAI, CONTROLLER_GLADIATOR))
		return new CStandardShipAI;
	else if (strEquals(sAI, CONTROLLER_FLEET_COMMAND))
		return new CFleetCommandAI;
	else if (strEquals(sAI, CONTROLLER_GAIAN_PROCESSOR))
		return new CGaianProcessorAI;
	else if (strEquals(sAI, CONTROLLER_ZOANTHROPE))
		return new CZoanthropeAI;
	else
		return NULL;
	}

DWORD ExtensionVersionToInteger (DWORD dwVersion)
	{
	switch (dwVersion)
		{
		//	Version 1.1
		case 3:
			return 11;

		//	Version 1.0
		case 2:
			return 10;

		//	Prior to 1.0
		case 1:
		case 0:
			return 9;

		//	Must handle all cases
		default:
			ASSERT(false);
			return 0;
		}
	}

CString GenerateRandomName (const CString &sList, const CString &sSubst)

//	GenerateRandomName
//
//	Generates a random name based on a list

	{
	int iCount = strDelimitCount(sList, ';', DELIMIT_TRIM_WHITESPACE);
	CString sName = strDelimitGet(sList, ';', DELIMIT_TRIM_WHITESPACE, mathRandom(0, iCount-1));
	return GenerateRandomNameFromTemplate(sName, sSubst);
	}

CString GenerateRandomNameFromTemplate (const CString &sName, const CString &sSubst)

//	GenerateRandomNameFromTemplate
//
//	Generates a random name by appropriate substitution

	{
	//	Substitute string

	char szResult[1024];
	char *pDest = szResult;
	char *pPos = sName.GetASCIIZPointer();

	while (*pPos)
		{
		switch (*pPos)
			{
			case '%':
				{
				pPos++;
				switch (*pPos)
					{
					case '%':
						*pDest++ = *pPos++;
						break;

					case 's':
						{
						char *pCopy = sSubst.GetASCIIZPointer();
						while (*pCopy)
							*pDest++ = *pCopy++;

						pPos++;
						break;
						}

					case '0':
						*pDest++ = '0' + mathRandom(0, 9);
						pPos++;
						break;

					case '1':
						*pDest++ = '1' + mathRandom(0, 8);
						pPos++;
						break;

					case 'A':
						*pDest++ = 'A' + mathRandom(0, 25);
						pPos++;
						break;

					case 'a':
						*pDest++ = 'a' + mathRandom(0, 25);
						pPos++;
						break;
					}

				break;
				}

			default:
				*pDest++ = *pPos++;
			}
		}

	//	Done

	*pDest = '\0';
	return CString(szResult);
	}

int GetDamageTypeLevel (DamageTypes iDamage)

//	GetDamageTypeLevel
//
//	Returns the level for the given damage type
//
//	laser/particle =		1
//	particle/blast =		4
//	ion/thermo =			7
//	etc.

	{
	return 1 + 3 * ((int)iDamage / 2);
	}

CString GetDamageResultsName (EDamageResults iResult)

//	GetDamageResultsName
//
//	Returns the name of the damage result

	{
	if (iResult < DAMAGE_RESULTS_COUNT)
		return CString(g_pszDamageResults[iResult]);
	else
		{
		ASSERT(false);
		return NULL_STR;
		}
	}

Metric *GetDestinyToBellCurveArray (void)

//	GetDestinyToBellCurveArray
//
//	Returns an array that maps a destiny value (0-359) to a random
//	number from -1.0 to 1.0 (with most of the values clustered around 0.0
//
//	We use an approximation of a cumulative distribution function
//	http://en.wikipedia.org/wiki/Error_function

	{
	int i;

	if (!g_bDestinyToBellCurveInit)
		{
		//	Some constants that make things work out to the right range

		Metric rMagicConstant = 0.147f;		//	From the approximation formula
		Metric rScale1 = 360.0f;
		Metric rScale2 = 3.7f;
		Metric r4OverPi = 4.0f / g_Pi;

		for (i = 0; i < g_DestinyRange; i++)
			{
			Metric rX = ((Metric)i - ((Metric)(g_DestinyRange - 1) / 2.0f)) / rScale1;
			Metric rX2 = rX * rX;
			Metric raX2 = rMagicConstant * rX2;
			Metric rSign = (rX > 0.0f ? 1.0f : -1.0f);

			g_pDestinyToBellCurve[i] = (rSign * (1.0f - exp(-rX2 * (r4OverPi + raX2) / (1.0f + raX2)))) * rScale2;
			}

		g_bDestinyToBellCurveInit = true;
		}

	return g_pDestinyToBellCurve;
	}

DestructionTypes GetDestructionCause (const CString &sString)

//	GetDestructionCause
//
//	Loads a destruction cause

	{
	int i;

	for (i = 0; i < DESTRUCTION_CAUSES_COUNT; i++)
		if (strEquals(sString, CString(g_pszDestructionCauses[i])))
			return (DestructionTypes)i;

	return killedNone;
	}

CString GetDestructionName (DestructionTypes iCause)

//	GetDestructionName
//
//	Returns the name of the destruction cause

	{
	if (iCause < DESTRUCTION_CAUSES_COUNT)
		return CString(g_pszDestructionCauses[iCause]);
	else
		{
		ASSERT(false);
		return NULL_STR;
		}
	}

int GetDiceCountFromAttribute(const CString &sValue)

//	GetDiceCountFromAttribute
//
//	If sValue is blank, return 1.
//	If sValue is a number, returns the number.
//	Otherwise, assumes that sValue is a dice range and
//	returns the random value.

	{
	if (sValue.IsBlank())
		return 1;
	else
		{
		DiceRange Count;
		if (Count.LoadFromXML(sValue) != NOERROR)
			return 0;

		return Count.Roll();
		}
	}

int GetFrequency (const CString &sValue)

//	GetFrequency
//
//	Returns the frequency from an attribute value

	{
	if (strEquals(sValue, FREQUENCY_COMMON))
		return ftCommon;
	else if (strEquals(sValue, FREQUENCY_UNCOMMON))
		return ftUncommon;
	else if (strEquals(sValue, FREQUENCY_RARE))
		return ftRare;
	else if (strEquals(sValue, FREQUENCY_VERYRARE))
		return ftVeryRare;
	else
		return ftNotRandom;
	}

int GetFrequencyByLevel (const CString &sLevelFrequency, int iLevel)

//	GetFrequencyByLevel
//
//	Returns the frequency for a given level. The frequency is encoded in
//	a string as follows:
//
//	--cur v---- ----- ----- -----
//
//	Where:
//
//	-	Not encountered
//	c	common
//	u	uncommon
//	r	rare
//	v	very rare
//
//	The string is arranged in five sections of five characters each (for
//	human legibility).

	{
	char *pTable = sLevelFrequency.GetPointer();
	if (pTable && pTable[0] == '*')
		return ftCommon;

	ASSERT(iLevel >= 1 && iLevel <= MAX_ITEM_LEVEL);
	int iChar = iLevel + ((iLevel - 1) / 5) - 1;
	if (iChar >= sLevelFrequency.GetLength())
		return ftNotRandom;

	switch (pTable[iChar])
		{
		case 'c':
		case 'C':
			return ftCommon;

		case 'u':
		case 'U':
			return ftUncommon;

		case 'r':
		case 'R':
			return ftRare;

		case 'v':
		case 'V':
			return ftVeryRare;

		default:
			return ftNotRandom;
		}
	}

CString GetGenomeID (GenomeTypes iGenome)

//	GetGenomeID
//
//	Returns the genome id

	{
	if ((int)iGenome < 0 || (int)iGenome >= GENOME_COUNT)
		return NULL_STR;

	return CString(g_Genome[iGenome].pszID);
	}

CString GetGenomeName (GenomeTypes iGenome)

//	GetGenomeName
//
//	Returns the genome name

	{
	if ((int)iGenome < 0 || (int)iGenome >= GENOME_COUNT)
		return NULL_STR;

	return CString(g_Genome[iGenome].pszName);
	}

Metric GetScale (CXMLElement *pObj)

//	GetScale
//
//	Returns the scale for this element

	{
	CString sScale = pObj->GetAttribute(SCALE_ATTRIB);
	if (sScale.IsBlank())
		return LIGHT_SECOND;
	else if (strEquals(sScale, SCALE_AU))
		return g_AU;
	else if (strEquals(sScale, SCALE_LIGHT_MINUTE))
		return LIGHT_MINUTE;
	else if (strEquals(sScale, SCALE_PIXEL))
		return g_KlicksPerPixel;
	else
		return LIGHT_SECOND;
	}

CSpaceObject::InterSystemResults GetInterSystemResult (const CString &sString)

//	GetInterSystemResult
//
//	Returns an InterSystemResult from a string

	{
	if (strEquals(sString, INTER_SYSTEM_FOLLOW_PLAYER))
		return CSpaceObject::interFollowPlayer;
	else if (strEquals(sString, INTER_SYSTEM_WAIT_FOR_PLAYER))
		return CSpaceObject::interWaitForPlayer;
	else
		return CSpaceObject::interNoAction;
	}

CString GetItemCategoryName (ItemCategories iCategory)

//	GetItemCategoryName
//
//	Returns the category name

	{
	switch (iCategory)
		{
		case itemcatMisc:
			return CONSTLIT("miscellaneous");

		case itemcatArmor:
			return CONSTLIT("armor");

		case itemcatWeapon:
			return CONSTLIT("weapon");

		case itemcatMiscDevice:
			return CONSTLIT("device");

		case itemcatLauncher:
			return CONSTLIT("launcher");

		case itemcatNano:
			return CONSTLIT("(unused)");

		case itemcatReactor:
			return CONSTLIT("reactor");

		case itemcatShields:
			return CONSTLIT("shield generator");

		case itemcatCargoHold:
			return CONSTLIT("cargo hold");

		case itemcatFuel:
			return CONSTLIT("fuel");

		case itemcatMissile:
			return CONSTLIT("missile or ammo");

		case itemcatDrive:
			return CONSTLIT("drive");

		case itemcatUseful:
			return CONSTLIT("usable");

		default:
			return CONSTLIT("unknown type");
		}
	}

CString GetMessageID (MessageTypes iMessage)

//	GetMessageID
//
//	Returns string message ID

	{
	return CString(g_pszMessageID[iMessage]);
	}

MessageTypes GetMessageFromID (const CString &sID)

//	GetMessageFromID
//
//	Returns a message from a message ID

	{
	int i;

	for (i = 0; i < MESSAGE_ID_COUNT; i++)
		if (strEquals(sID, CString(g_pszMessageID[i])))
			return (MessageTypes)i;

	return msgNone;
	}

CString GetOrderName (IShipController::OrderTypes iOrder)

//	GetOrderName
//
//	Returns the name of the order

	{
	return CString(g_OrderTypes[iOrder].szName);
	}

int OrderGetDataCount (IShipController::OrderTypes iOrder)

//	OrderGetDataCount
//
//	Returns the number of data args

	{
	switch (g_OrderTypes[iOrder].szData[0])
		{
		case '-':
			return 0;

		case 'i':
			return 1;
		
		case '2':
			return 2;

		default:
			return 0;
		}
	}

bool OrderHasTarget (IShipController::OrderTypes iOrder, bool *retbRequired)

//	OrderHasTarget
//
//	Returns TRUE if the given order has a target

	{
	if (retbRequired)
		*retbRequired = (*g_OrderTypes[iOrder].szTarget == 'o');

	return (*g_OrderTypes[iOrder].szTarget != '-');
	}

IShipController::OrderTypes GetOrderType (const CString &sString)

//	GetOrderType
//
//	Loads an order type

	{
	int iType;

	for (iType = 0; iType < ORDER_TYPES_COUNT; iType++)
		if (strEquals(sString, CString(g_OrderTypes[iType].szName)))
			return (IShipController::OrderTypes)iType;

	return IShipController::orderNone;
	}

bool HasModifier (const CString &sModifierList, const CString &sModifier)

//	HadModifier
//
//	Returns TRUE if the item has the given modifier. sModifierList is a list
//	of semicolon (or comma) separated strings. sModifier is a string.

	{
#ifdef DEBUG_HASMODIFIER
	char szBuffer[1024];
	wsprintf(szBuffer, "Looking for: %s; Compare: %s\n", sModifier.GetPointer(), sModifierList.GetPointer());
	::OutputDebugString(szBuffer);
#endif

	//	Blank modifiers always win

	if (sModifier.IsBlank())
		return true;

	//	Loop over modifiers

	char *pPos = sModifierList.GetASCIIZPointer();
	while (*pPos != '\0')
		{
		//	Trim spaces

		while (*pPos == ' ')
			pPos++;

		//	Do we match the modifier?

		char *pFind = sModifier.GetASCIIZPointer();
		while (*pFind != '\0' && strLowerCaseAbsolute(*pFind) == strLowerCaseAbsolute(*pPos))
			{
			pFind++;
			pPos++;
			}

		//	If we matched, then we've got this modifier

		if (*pFind == '\0' && (*pPos == '\0' || *pPos == ';' || *pPos == ',' || *pPos == ' '))
			{
#ifdef DEBUG_HASMODIFIER
			char szBuffer[1024];
			wsprintf(szBuffer, "Looking for: %s; Found: %s\n", sModifier.GetPointer(), sModifierList.GetPointer());
			::OutputDebugString(szBuffer);
#endif
			return true;
			}

		//	Otherwise, skip to the next modifier

		while (*pPos != '\0' && *pPos != ';' && *pPos != ',' && *pPos != ' ')
			pPos++;

		if (*pPos == ';' || *pPos == ',')
			pPos++;
		}

	return false;
	}

bool IsConstantName (const CString &sList)

//	IsConstantName
//
//	Returns TRUE if there is only a single name in the list and
//	if the name has no substitution codes

	{
	char *pPos = sList.GetASCIIZPointer();
	while (*pPos != '\0')
		{
		if (*pPos == ';' || *pPos == '%')
			return false;

		pPos++;
		}

	return true;
	}

bool IsEnergyDamage (DamageTypes iType)
	{
	switch (iType)
		{
		case damageLaser:
		case damageParticle:
		case damageIonRadiation:
		case damagePositron:
		case damageAntiMatter:
		case damageGravitonBeam:
		case damageDarkAcid:
		case damageDarkLightning:
			return true;

		default:
			return false;
		}
	}

bool IsMatterDamage (DamageTypes iType)
	{
	switch (iType)
		{
		case damageKinetic:
		case damageBlast:
		case damageThermonuclear:
		case damagePlasma:
		case damageNano:
		case damageSingularity:
		case damageDarkSteel:
		case damageDarkFire:
			return true;

		default:
			return false;
		}
	}

ArmorCompositionTypes LoadArmorComposition (const CString &sString)

//	LoadArmorComposition
//
//	Load armor composition attribute

	{
	if (strEquals(sString, ARMOR_COMPOSITION_METALLIC))
		return compMetallic;
	else if (strEquals(sString, ARMOR_COMPOSITION_CERAMIC))
		return compCeramic;
	else if (strEquals(sString, ARMOR_COMPOSITION_CARBIDE))
		return compCarbide;
	else if (strEquals(sString, ARMOR_COMPOSITION_NANOSCALE))
		return compNanoScale;
	else if (strEquals(sString, ARMOR_COMPOSITION_QUANTUM))
		return compQuantum;
	else if (strEquals(sString, ARMOR_COMPOSITION_GRAVITIC))
		return compGravitic;
	else if (strEquals(sString, ARMOR_COMPOSITION_DARKMATTER))
		return compDarkMatter;
	else
		return compUnknown;
	}

ALERROR LoadCodeBlock (const CString &sCode, ICCItem **retpCode, CString *retsError)

//	LoadCodeBlock
//
//	Loads a block of code

	{
	//	Null case

	if (sCode.IsBlank())
		{
		*retpCode = NULL;
		return NOERROR;
		}

	//	Compile the code

	ICCItem *pCode = g_pUniverse->GetCC().Link(sCode, 0, NULL);
	if (pCode->IsError())
		{
		if (retsError)
			*retsError = pCode->GetStringValue();

		pCode->Discard(&g_pUniverse->GetCC());
		return ERR_FAIL;
		}

	//	Done

	*retpCode = pCode;
	return NOERROR;
	}

ALERROR LoadDamageAdj (CXMLElement *pItem, const CString &sAttrib, int *retiAdj, int *retiCount)

//	LoadDamageAdj
//
//	Loads an attribute of the form "x1,x2,x3,..."
//	into an array of damage adjustments.

	{
	ALERROR error;
	int i;
	CIntArray Adj;

	if (error = pItem->GetAttributeIntegerList(sAttrib, &Adj))
		return error;

	for (i = 0; i < damageCount; i++)
		retiAdj[i] = (i < Adj.GetCount() ? Adj.GetElement(i) : 0);

	if (retiCount)
		*retiCount = Adj.GetCount();

	return NOERROR;
	}

ALERROR LoadDamageAdj (CXMLElement *pDesc, int *pDefAdj, int *retiAdj)

//	LoadDamageAdj
//
//	Loads either the damageAdj or the hpBonus attributes into a damage adjument array

	{
	ALERROR error;
	int i;
	CString sValue;

	if (pDesc->FindAttribute(HP_BONUS_ATTRIB, &sValue))
		{
		char *pPos = sValue.GetASCIIZPointer();
		int iDamage = 0;

		while (iDamage < damageCount)
			{
			if (*pPos != '\0')
				{
				//	Skip whitespace

				while (*pPos == ' ')
					pPos++;

				//	A star means no damage

				if (*pPos == '*' || pDefAdj[iDamage] <= 0)
					retiAdj[iDamage] = 0;

				//	Otherwise, expect a number

				else
					{
					bool bNull;
					int iValue;
					iValue = strParseInt(pPos, 0, &pPos, &bNull);
					if (bNull)
						return ERR_FAIL;

					int iInc = iValue + (10000 / pDefAdj[iDamage]) - 100;
					if (iInc > -100)
						retiAdj[iDamage] = 10000 / (100 + iInc);
					else
						retiAdj[iDamage] = 10000000;
					}

				//	Skip until separator

				while (*pPos != ',' && *pPos != ';' && *pPos != '\0')
					pPos++;

				if (*pPos != '\0')
					pPos++;
				}
			else
				retiAdj[iDamage] = pDefAdj[iDamage];

			iDamage++;
			}
		}
	else if (pDesc->FindAttribute(DAMAGE_ADJ_ATTRIB, &sValue))
		{
		CIntArray DamageAdj;
		if (error = ParseAttributeIntegerList(sValue, &DamageAdj))
			return error;

		for (i = 0; i < damageCount; i++)
			if (i < DamageAdj.GetCount())
				retiAdj[i] = DamageAdj.GetElement(i);
			else
				retiAdj[i] = 100;
		}
	else
		{
		for (i = 0; i < damageCount; i++)
			retiAdj[i] = pDefAdj[i];
		}

	return NOERROR;
	}

DWORD LoadExtensionVersion (const CString &sVersion)

//	LoadExtensionVersion
//
//	Returns the extension version (or 0 if this is an unrecognized extension)

	{
	if (strEquals(sVersion, VERSION_110))
		//	Latest version, currently 3.
		//	See: TSEUtil.h for definition.
		return EXTENSION_VERSION;
	else if (strEquals(sVersion, VERSION_097)
			|| strEquals(sVersion, VERSION_097A)
			|| strEquals(sVersion, VERSION_098)
			|| strEquals(sVersion, VERSION_098A)
			|| strEquals(sVersion, VERSION_098B)
			|| strEquals(sVersion, VERSION_098C)
			|| strEquals(sVersion, VERSION_098D)
			|| strEquals(sVersion, VERSION_099)
			|| strEquals(sVersion, VERSION_099A)
			|| strEquals(sVersion, VERSION_099B)
			|| strEquals(sVersion, VERSION_099C)
			|| strEquals(sVersion, VERSION_100))
		return 2;
	else if (strEquals(sVersion, VERSION_095)
			|| strEquals(sVersion, VERSION_095A)
			|| strEquals(sVersion, VERSION_095B)
			|| strEquals(sVersion, VERSION_096)
			|| strEquals(sVersion, VERSION_096A))
		return 1;
	else
		return 0;
	}

DWORD LoadNameFlags (CXMLElement *pDesc)

//	LoadNameFlags
//
//	Returns flags word with NounFlags

	{
	DWORD dwFlags = 0;

	if (pDesc->GetAttributeBool(DEFINITE_ARTICLE_ATTRIB))
		dwFlags |= nounDefiniteArticle;
	if (pDesc->GetAttributeBool(FIRST_PLURAL_ATTRIB))
		dwFlags |= nounFirstPlural;
	if (pDesc->GetAttributeBool(ES_PLURAL_ATTRIB))
		dwFlags |= nounPluralES;
	if (pDesc->GetAttributeBool(CUSTOM_PLURAL_ATTRIB))
		dwFlags |= nounCustomPlural;
	if (pDesc->GetAttributeBool(SECOND_PLURAL_ATTRIB))
		dwFlags |= nounSecondPlural;
	if (pDesc->GetAttributeBool(VOWEL_ARTICLE_ATTRIB))
		dwFlags |= nounVowelArticle;
	if (pDesc->GetAttributeBool(NO_ARTICLE_ATTRIB))
		dwFlags |= nounNoArticle;
	if (pDesc->GetAttributeBool(PERSONAL_NAME_ATTRIB))
		dwFlags |= nounPersonalName;

	return dwFlags;
	}

WORD LoadRGBColor (const CString &sString)

//	LoadRGBColor
//
//	Returns a 16-bit color from an RGB triplet

	{
	char *pPos = sString.GetASCIIZPointer();
	int iRed = strParseInt(pPos, 0, &pPos, NULL); if (*pPos) pPos++;
	int iGreen = strParseInt(pPos, 0, &pPos, NULL); if (*pPos) pPos++;
	int iBlue = strParseInt(pPos, 0, &pPos, NULL);

	return CG16bitImage::RGBValue(iRed, iGreen, iBlue);
	}

COLORREF LoadCOLORREF (const CString &sString)

//	LoadCOLORREF
//
//	Returns a 32-bit color from an RGB triplet

	{
	char *pPos = sString.GetASCIIZPointer();
	int iRed = strParseInt(pPos, 0, &pPos, NULL); if (*pPos) pPos++;
	int iGreen = strParseInt(pPos, 0, &pPos, NULL); if (*pPos) pPos++;
	int iBlue = strParseInt(pPos, 0, &pPos, NULL);

	return RGB(iRed, iGreen, iBlue);
	}

DWORD LoadUNID (SDesignLoadCtx &Ctx, const CString &sString)

//	LoadUNID
//
//	Returns an UNID either relative (@xxx) or absolute

	{
	if (Ctx.pExtension)
		{
		char *pPos = sString.GetASCIIZPointer();
		if (*pPos == '@')
			{
			WORD wLow = LOWORD(Ctx.pExtension->dwUNID) + (WORD)strParseIntOfBase(pPos+1, 16, 0, NULL, NULL);
			return MAKELONG(wLow, HIWORD(Ctx.pExtension->dwUNID));
			}
		else
			return strToInt(sString, 0, NULL);
		}
	else
		return strToInt(sString, 0, NULL);
	}

int NLCompare (TArray<CString> &Input, TArray<CString> &Pattern)

//	NLCompare
//
//	Compares the two word collection.
//
//	For each word in Input that matches a word in Pattern
//	exactly, we return +10
//
//	For each word in Input that matches a word in Pattern
//	partially, we return +2
//
//	For each word in Pattern in excess of the number of words
//	in Input, we return -1

	{
	int i, j;
	int iResult = 0;

	for (i = 0; i < Input.GetCount(); i++)
		{
		//	If this word in the input exactly matches one of the words
		//	in the pattern, then +10

		bool bMatched = false;
		for (j = 0; j < Pattern.GetCount(); j++)
			if (strEquals(Input[i], Pattern[j]))
				{
				bMatched = true;
				iResult += 10;
				break;
				}

		if (bMatched)
			continue;

		//	If this word in the input is a number, and if the number as
		//	a roman numeral matches one of the words in the pattern
		//	exactly, then +5

		int iNumber = strToInt(Input[i], -1, NULL);
		if (iNumber > 0)
			{
			CString sRomanNumeral = strRomanNumeral(iNumber);
			for (j = 0; j < Pattern.GetCount(); j++)
				if (strEquals(sRomanNumeral, Pattern[j]))
					{
					bMatched = true;
					iResult += 5;
					break;
					}
			}

		if (bMatched)
			continue;

		//	If this word in the input partially matches one of the words
		//	in the pattern, then +2

		for (j = 0; j < Pattern.GetCount(); j++)
			if (strFind(Pattern[j], Input[i]) != -1)
				{
				bMatched = true;
				iResult += 2;
				break;
				}

		if (bMatched)
			continue;
		}

	//	Adjust for length

	if (Input.GetCount() < Pattern.GetCount())
		iResult -= 1;

	return iResult;
	}

void NLSplit (const CString &sPhrase, TArray<CString> *retArray)

//	NLSplit
//
//	Splits a phrase into words for comparison

	{
	//	Generate a list of strings that we will search for

	char *pPos = sPhrase.GetASCIIZPointer();
	CString sToken;
	CString sPrevAppend;
	while (true)
		{
		if ((*pPos >= 'a' && *pPos <= 'z')
				|| (*pPos >= 'A' && *pPos <= 'Z')
				|| (*pPos >= '0' && *pPos <= '9'))
			{
			sToken.Append(CString(pPos, 1));
			}
		else
			{
			if (!sToken.IsBlank())
				{
				//	Add this word to the list

				retArray->Insert(sToken);

				//	If we had a hyphen previously, then we append the
				//	concatenation

				if (!sPrevAppend.IsBlank())
					{
					sPrevAppend.Append(sToken);
					retArray->Insert(sPrevAppend);
					sPrevAppend = NULL_STR;
					}

				//	If this is a hyphen, then remember this word for
				//	future appending

				if (*pPos == '-')
					sPrevAppend = sToken;

				//	Reset token

				sToken = NULL_STR;
				}

			//	If this is the end, then we're done

			if (*pPos == '\0')
				break;
			}

		pPos++;
		}
	}

CString ParseCriteriaParam (char **ioPos, bool bExpectColon, bool *retbBinaryParam)

//	ParseCriteriaParam
//
//	Parses a criteria parameter. *ioPos starts by pointing at the
//	criteria character right before the start of the parameter.
//	We leave it set to the last character of the parameter
//	(ready to be incremented).
//
//	If bExpectColon is set, then we parse a param of the form:
//
//		:param;
//
//	In which we strip out the leading colon; In this mode, if we don't see
//	a leading colon, we assume a NULL_STR param (this syntax supports
//	optional params)
//
//	If the param has an embedded colon, we return retbBinaryParam set to TRUE.

	{
	char *pPos = *ioPos;

	//	Skip criteria character

	pPos++;

	//	Skip whitespace

	while (*pPos == ' ' || *pPos == '\t')
		pPos++;

	//	If we expect a colon, process it now

	if (bExpectColon)
		{
		//	If we don't have a colon, then we return
		//	NULL_STR and leave the pointer where it was. This takes
		//	care of the case where we have no parameter.

		if (*pPos != ':')
			{
			if (retbBinaryParam)
				*retbBinaryParam = false;
			return NULL_STR;
			}

		//	Otherwise, consume colon

		pPos++;
		}

	//	Parse the parameter

	bool bBinaryParam = false;
	char *pStart = pPos;
	while (*pPos != ';' && *pPos != ' ' && *pPos != '\t' && *pPos != '\0')
		{
		if (*pPos == ':')
			bBinaryParam = true;
		pPos++;
		}

	//	If we hit the end, we backup one character because our caller
	//	will advance the position by one

	*ioPos = (*pPos == '\0' ? (pPos - 1) : pPos);

	//	Done

	if (retbBinaryParam)
		*retbBinaryParam = bBinaryParam;

	return CString(pStart, pPos - pStart);
	}

GenomeTypes ParseGenomeID (const CString &sText)

//	ParseGenomeID
//
//	Returns the genome from an ID

	{
	int i;

	for (i = 0; i < GENOME_COUNT; i++)
		if (strEquals(sText, CString(g_Genome[i].pszID)))
			return (GenomeTypes)i;

	return genomeUnknown;
	}

CString ParseNounForm (const CString &sNoun, DWORD dwNounFlags, bool bPluralize, bool bShortName)

//	ParseNounForm
//
//	Parses a string of the form:
//
//	[abc]def| [hij]klm
//
//	Using the noun flags and the required result, it parses out the various component
//	of the noun (plural form, short name) and returns the required noun

	{
	char *pPos = sNoun.GetASCIIZPointer();

	CString sDest;
	char *pDest = sDest.GetWritePointer(sNoun.GetLength() + 10);

	bool bDestIsSingular = true;
	bool bInPluralSect = false;
	bool bInLongNameSect = false;
	bool bSkipWhitespace = true;
	bool bStartOfWord = true;
	int iWord = 1;

	while (*pPos != '\0')
		{
		//	Skip whitespace until we find non-whitespace

		if (bSkipWhitespace)
			{
			if (*pPos == ' ')
				{
				pPos++;
				continue;
				}
			else
				bSkipWhitespace = false;
			}

		//	Begin and end plural addition. Everything inside parentheses
		//	is added only if plural

		if (*pPos == '(' && !bStartOfWord)
			{
			bInPluralSect = true;

			//	If we have a paren, then it means that we can form the
			//	plural version. We don't need any additional auto pluralization

			if (bPluralize)
				bDestIsSingular = false;
			}
		else if (*pPos == ')' && bInPluralSect)
			bInPluralSect = false;

		//	Begin and end long name section. Everything inside
		//	brackets appears only for long names

		else if (*pPos == '[')
			bInLongNameSect = true;
		else if (*pPos == ']')
			bInLongNameSect = false;

		//	Escape code

		else if (*pPos == '\\')
			{
			pPos++;	if (*pPos == '\0') break;

			//	Deal with escape codes

			if (*pPos == 'x' || *pPos == 'X')
				{
				pPos++;	if (*pPos == '\0') break;
				int iFirstDigit = strGetHexDigit(pPos);

				pPos++;	if (*pPos == '\0') break;
				int iSecondDigit = strGetHexDigit(pPos);

				if ((!bInLongNameSect || !bShortName)
						&& (!bInPluralSect || bPluralize))
					*pDest++ = (char)(16 * iFirstDigit + iSecondDigit);
				}

			//	Add, but not if we're in the long names section
			//	and we only want a short name

			else if ((!bInLongNameSect || !bShortName)
					&& (!bInPluralSect || bPluralize))
				*pDest++ = *pPos;
			}

		//	A semi-colon or vertical-bar means that we have two
		//	name: singular followed by plural

		else if (*pPos == ';' || *pPos == '|')
			{
			//	If we don't need to plural form, then we've reached
			//	the end of the singular form, so we're done

			if (!bPluralize)
				break;

			//	Reset the destination so we start capturing from
			//	the plural section

			pDest = sDest.GetASCIIZPointer();
			bDestIsSingular = false;
			bSkipWhitespace = true;
			}
		else
			{
			//	If we've reached the end of a word, see if we need
			//	to add a plural

			if (*pPos == ' ')
				{
				if (bPluralize && bDestIsSingular && !bShortName)
					{
					if ((iWord == 1 && (dwNounFlags & nounFirstPlural))
							|| (iWord == 2 && (dwNounFlags & nounSecondPlural)))
						{
						if (dwNounFlags & nounPluralES)
							{
							*pDest++ = 'e';
							*pDest++ = 's';
							}
						else
							*pDest++ = 's';

						bDestIsSingular = false;
						}
					}

				iWord++;
				bSkipWhitespace = true;
				bStartOfWord = true;
				}
			else
				bStartOfWord = false;

			if ((!bInLongNameSect || !bShortName)
					&& (!bInPluralSect || bPluralize))
				*pDest++ = *pPos;
			}

		pPos++;
		}

	//	Add plural if necessary (short name implies plural because
	//	a short name is always a quantifiable item, e.g., a *mass of* concrete)

	if (bPluralize && bDestIsSingular && !bShortName)
		{
		if (dwNounFlags & nounPluralES)
			{
			*pDest++ = 'e';
			*pDest++ = 's';
			}
		else
			*pDest++ = 's';
		}

	//	Done

	*pDest++ = '\0';
	sDest.Truncate(pDest - sDest.GetASCIIZPointer() - 1);
	return sDest;
	}

void ParseStringList (const CString &sList, DWORD dwFlags, TArray<CString> *retList)

//	ParseStringList
//
//	Splits a string into a list of strings

	{
	char *pPos = sList.GetASCIIZPointer();
	while (*pPos != '\0')
		{
		//	Trim spaces

		while (strIsWhitespace(pPos))
			pPos++;

		//	Skip until delimiter

		char *pStart = pPos;
		while (*pPos != '\0' && *pPos != ';' && *pPos != ',' && !strIsWhitespace(pPos))
			pPos++;

		//	Insert modifier

		if (pPos != pStart)
			retList->Insert(CString(pStart, pPos - pStart));

		//	Next

		if (*pPos == ';' || *pPos == ',')
			pPos++;
		}
	}

Metric RangeIndex2Range (int iIndex)

//	RangeIndex2Range
//
//	Returns the range for the given range index (0-8)

	{
	ASSERT(iIndex >= 0 && iIndex < RANGE_INDEX_COUNT);
	return g_RangeIndex[iIndex];
	}

CString ReactorPower2String (int iPower)

//	ReactorPower2String
//
//	Generates a string for reactor power

	{
	int iReactorPower = (iPower + 5) / 10;

	if (iReactorPower < 1000)
		return strPatternSubst(CONSTLIT("%dMW"), iReactorPower);
	else
		{
		int iGW = iReactorPower / 1000;
		int iMW = iReactorPower % 1000;

		if (iMW == 0)
			return strPatternSubst(CONSTLIT("%dGW"), iGW);
		else
			return strPatternSubst(CONSTLIT("%d.%dGW"),	iGW, (iMW + 50) / 100);
		}
	}

void ReportCrashObj (CString *retsMessage, CSpaceObject *pCrashObj)

//	ReportCrashObj
//
//	Include information about the given object

	{
	CString sName = CONSTLIT("unknown");
	CString sType = CONSTLIT("unknown");
	bool bDestroyed = false;
	CString sData;

	if (pCrashObj == NULL)
		pCrashObj = g_pProgramObj;

	if (pCrashObj == NULL)
		{
		retsMessage->Append(CONSTLIT("obj NULL\r\n"));
		return;
		}

	if (pCrashObj)
		{
		try
			{
			sName = pCrashObj->GetName();
			sType = pCrashObj->GetObjClassName();
			bDestroyed = pCrashObj->IsDestroyed();
			}
		catch (...)
			{
			}

		try
			{
			sData = pCrashObj->DebugCrashInfo();
			}
		catch (...)
			{
			sData = CONSTLIT("error obtaining crash info\r\n");
			}
		}

	if (bDestroyed)
		retsMessage->Append(CONSTLIT("obj destroyed\r\n"));
	retsMessage->Append(strPatternSubst(CONSTLIT("obj class: %s\r\n"), sType));
	retsMessage->Append(strPatternSubst(CONSTLIT("obj name: %s\r\n"), sName));
	retsMessage->Append(strPatternSubst(CONSTLIT("obj pointer: %x\r\n"), pCrashObj));
	retsMessage->Append(sData);
	}
