#ifndef SENSORS_H
#define SENSORS_H

#include <velib/base/base.h>
#include <velib/types/ve_item.h>
#include <velib/types/ve_item_def.h>

#include "adc.h"
#include "values.h"

// defines for the on-board sensors interface configurations to firmware application
#define NUM_OF_SENSOR_SETTINGS_PARAMS		4
#define NUM_OF_PROD_ITEMS					4
#define NUM_OF_SENSOR_OUTPUTS				3
#define NUM_OF_SENSOR_VARIANTS				NUM_OF_SENSOR_OUTPUTS + NUM_OF_SENSOR_SETTINGS_PARAMS
#define SENSORS_INFO_ARRAY_SIZE				NUM_OF_SENSOR_VARIANTS + NUM_OF_PROD_ITEMS

// defines for the tank level sensor analog front end parameters
#define TANK_LEVEL_SENSOR_DIVIDER			(680) // ohms
#define EUR_MAX_TANK_LEVEL_RESISTANCE		(180) //ohms
#define USA_MAX_TANK_LEVEL_RESISTANCE		(240) //ohms
#define USA_MIN_TANK_LEVEL_RESISTANCE		(30) //ohms

// defines for the temperature sensor analog front end parameters
#define TEMP_SENS_VOLT_DIVID_R1				(10000) // ohms
#define TEMP_SENS_VOLT_DIVID_R2				(4700) // ohms
#define KELVIN_2_CELCIUS(X)					(X+273)
#define TEMP_SENS_MAX_ADCIN					ADC_1p3VOLTS // ~400K
#define TEMP_SENS_MIN_ADCIN					ADC_0p8VOLTS // ~(-22) dgrees C
#define TEMP_SENS_S_C_ADCIN					50 // samples
#define TEMP_SENS_INV_PLRTY_ADCIN			ADC_0p208VOLTS // 0.7 volts at divider input
#define TEMP_SENS_INV_PLRTY_ADCIN_BAND		ADC_0p15VOLTS
#define TEMP_SENS_INV_PLRTY_ADCIN_LB		(TEMP_SENS_INV_PLRTY_ADCIN - TEMP_SENS_INV_PLRTY_ADCIN_BAND)
#define TEMP_SENS_INV_PLRTY_ADCIN_HB		(TEMP_SENS_INV_PLRTY_ADCIN + TEMP_SENS_INV_PLRTY_ADCIN_BAND)
#define VALUE_BETWEEN(V,L,H)				((((L)<(V))&&((V)<(H)))?1:0)

// defines to reset filter values
#define INIT_ADC_SAMPLE_VAL					0
#define INIT_ADC_SAMPLE_MEMORY_VAL			0
// defines to tank level sensor filter parameters
#define TANK_SENSOR_IIR_LPF_FF_VALUE		1000	// samples
#define TANK_SENSOR_CUTOFF_FREQ				0.001	// Hz
// defines to temperature sensor filter parameters
#define TEMPERATURE_SENSOR_IIR_LPF_FF_VALUE	500		// samples
#define TEMPERATURE_SENSOR_CUTOFF_FREQ		0.01		// Hz

// defines to initialize the sensors settings parameters
// tank level capacity
#define DEFAULT_TANK_CAPACITY				(float)0.2 //m3
#define MIN_OF_TANK_CAPACITY				0
#define MAX_OF_TANK_CAPACITY				1000
// tank level fluid type
#define DEFAULT_FLUID_TYPE					0 // Fuel
#define MIN_OF_FLUID_TYPE					0
#define MAX_OF_FLUID_TYPE					5 //
// temperature sensor signal sacle correction
#define TEMPERATURE_SCALE					(float)1.00
#define MIN_OF_TEMPERATURE_SCALE			(float)0.10
#define MAX_OF_TEMPERATURE_SCALE			(float)10.00
// temperature sensor signal offset correction
#define TEMPERATURE_OFFSET					0
#define MIN_OF_TEMPERATURE_OFFSET			-100
#define MAX_OF_TEMPERATURE_OFFSET			100

// analog input function
typedef enum {
	no_function = 0,
	default_function,
	num_of_functions
} sensor_function_t;

// index of sensors settings parameters array
typedef enum {
	analogpinFunc = 0,
	capacity = 1,
	fluidType = 2,
	standard = 3,
	scale = 1,
	offset = 2,
	TempType = 3,
	num_of_parameters = 4
} parameter_name_t;

// sensors array index
typedef enum {
	index_tankLevel1 = 0,
	index_tankLevel2,
	index_tankLevel3,
	index_temperature1,
	index_temperature2,
	num_of_analog_sensors
} analog_sensors_index_t;

// sensor statuses
typedef enum {
	ok = 0,
	disconnected,
	short_circuited,
	reverse_polarity,
	unknown_value,
	num_of_sensor_statuses
} sensor_status_t;

// tank level sensor standards that the app can handle
typedef enum {
	european_std = 0,
	american_std,
	num_of_stds
} tank_sensor_std_t;

// type of temperature sensors that the app can handle
typedef enum {
	battery = 0,
	refrigerator,
	other,
	num_of_temperature_sensor_type
} temperature_sensor_type_t;
#define DEFAULT_TEMPERATURE_TYPE	battery
#define MIN_TEMPERATURE_TYPE		0

// types of sensors that the app can handle
typedef enum {
	tank_level_t = 0,
	temperature_t,
	num_of_sensorsTypes
} sensors_type_t;

// sensor product items for dbus service
typedef struct {
	VeItem name;
	VeItem id;
	VeItem instance;
	VeItem connected;
} ProductInfo;

// snesors items for dbus tank level sensor service
typedef struct {
	VeItem level;
	VeItem remaining;
	VeItem status;
	VeItem analogpinFunc;
	VeItem capacity;
	VeItem fluidType;
	VeItem standard;
} tank_level_sensor_item_t;

// snesors items for dbus temperature sensor service
typedef struct {
	VeItem temperature;
	VeItem status;
	VeItem analogpinFunc;
	VeItem scale;
	VeItem offset;
	VeItem temperatureType;
	VeItem spareParam;
} temperature_sensor_item_t;

// snesors variables for tank level sensor items
typedef struct {
	VeVariant level;
	VeVariant remaining;
	VeVariant status;
	VeVariant analogpinFunc;
	VeVariant capacity;
	VeVariant fluidType;
	VeVariant standard;
} tank_level_sensor_variant_t;

// snesors variables for temperature sensor items
typedef struct {
	VeVariant temperature;
	VeVariant status;
	VeVariant analogpinFunc;
	VeVariant scale;
	VeVariant offset;
	VeVariant temperatureType;
	VeVariant spareParam;
} temperature_sensor_variant_t;

// paramters to interface the sensor to dbus service
typedef struct {
	const char *service;
	veBool connected;
} sensors_dbus_interface_t;

// building a sensor items structure to be published to dbus service
typedef struct {
	ProductInfo product;
	union {
		tank_level_sensor_item_t tank_level;
		temperature_sensor_item_t temperature;
	};
} sensors_items_t;

// sensor item variables per sensor type
typedef struct {
	union {
		tank_level_sensor_variant_t tank_level;
		temperature_sensor_variant_t temperature;
	};
} sensors_variants_t;

// sensor signal correction parameters
typedef struct {
	float scale;
	float offset;
} signal_correction_t;

// building a sensor signal conditioning structure
typedef struct {
	signal_correction_t sig_correct;
	filter_iir_lpf_t filter_iir_lpf;
} signal_condition_t;

// building a sensor interface structure
typedef struct {
	const adc_analogPin_t adc_pin;
	un32 adc_sample;
	signal_condition_t sig_cond;
	sensors_dbus_interface_t dbus;
} sensors_interface_t;

// building a sensor structure
typedef struct {
	const sensors_type_t sensor_type;
	veBool valid;
	sensors_interface_t interface;
	dbus_info_t dbus_info[NUM_OF_SENSOR_SETTINGS_PARAMS];
	sensors_items_t items;
	sensors_variants_t variant;
} analog_sensor_t;

/**
 * @brief sensor_init
 * @param root
 * @param sensor_index
 */
void sensor_init(VeItem *root, analog_sensors_index_t sensor_index);
/**
 * @brief sensors_handle
 */
void sensors_handle(void);
/**
 * @brief sensors_addSettings
 * @param sensor_index
 */
void sensors_addSettings(analog_sensors_index_t sensor_index);
/**
 * @brief sensors_dbusInit
 * @param sensor_index
 */
void sensors_dbusInit(analog_sensors_index_t sensor_index);
/**
 * @brief values_dbus_service_addSettings
 * @param sensor
 */
void values_dbus_service_addSettings(analog_sensor_t *sensor);
/**
 * @brief sensors_dbusConnect
 * @param sensor
 * @param sensor_index
 */
void sensors_dbusConnect(analog_sensor_t *sensor, analog_sensors_index_t sensor_index);
/**
 * @brief sensors_dbusDisconnect
 * @param sensor
 * @param sensor_index
 */
void sensors_dbusDisconnect(analog_sensor_t *sensor, analog_sensors_index_t sensor_index);

// a define to hold all the required predetermind variables and constants of the analog sensor structure
#define SENSORS_CONSTANT_DATA \
{		\
	{	\
		tank_level_t,\
		veFalse,\
		{\
			adc_pin4,\
			INIT_ADC_SAMPLE_VAL,\
			{{},{TANK_SENSOR_IIR_LPF_FF_VALUE, TANK_SENSOR_CUTOFF_FREQ, INIT_ADC_SAMPLE_MEMORY_VAL}},\
			{\
				"com.victronenergy.tank.builtin_adc4_di0",\
				veFalse\
			}		\
		},		\
		{\
			{\
				default_function,\
				no_function,\
				(num_of_functions-1),\
				"Settings/AnalogInput/Resistive/1/Function"\
			},\
			{\
				DEFAULT_TANK_CAPACITY,\
				MIN_OF_TANK_CAPACITY,\
				MAX_OF_TANK_CAPACITY,\
				"Settings/Tank/1/Capacity"\
			},\
			{\
				DEFAULT_FLUID_TYPE,\
				MIN_OF_FLUID_TYPE,\
				MAX_OF_FLUID_TYPE,\
				"Settings/Tank/1/FluidType"\
			},\
			{\
				european_std,\
				european_std,\
				(num_of_stds - 1),\
				"Settings/Tank/1/Standard"\
			}\
		}\
	},\
	{\
		tank_level_t,\
		veFalse,\
		{\
			adc_pin6,\
			INIT_ADC_SAMPLE_VAL,\
			{{},{TANK_SENSOR_IIR_LPF_FF_VALUE, TANK_SENSOR_CUTOFF_FREQ, INIT_ADC_SAMPLE_MEMORY_VAL}},\
			{\
				"com.victronenergy.tank.builtin_adc6_di1",\
				veFalse\
			}\
		},\
		{\
			{\
				default_function,\
				no_function,\
				(num_of_functions-1),\
				"Settings/AnalogInput/Resistive/2/Function"\
			},\
			{\
				DEFAULT_TANK_CAPACITY,\
				MIN_OF_TANK_CAPACITY,\
				MAX_OF_TANK_CAPACITY,\
				"Settings/Tank/2/Capacity"\
			},\
			{\
				DEFAULT_FLUID_TYPE,\
				MIN_OF_FLUID_TYPE,\
				MAX_OF_FLUID_TYPE,\
				"Settings/Tank/2/FluidType"\
			},\
			{\
				european_std,\
				european_std,\
				(num_of_stds - 1),\
				"Settings/Tank/2/Standard"\
			}\
		}\
	},\
	{\
		tank_level_t,\
		veFalse,\
		{\
			adc_pin2,\
			INIT_ADC_SAMPLE_VAL,\
			{{},{TANK_SENSOR_IIR_LPF_FF_VALUE, TANK_SENSOR_CUTOFF_FREQ, INIT_ADC_SAMPLE_MEMORY_VAL}},\
			{\
				"com.victronenergy.tank.builtin_adc2_di2",\
				veFalse\
			}\
		},\
		{\
			{\
				default_function,\
				no_function,\
				(num_of_functions-1),\
				"Settings/AnalogInput/Resistive/3/Function"\
			},\
			{\
				DEFAULT_TANK_CAPACITY,\
				MIN_OF_TANK_CAPACITY,\
				MAX_OF_TANK_CAPACITY,\
				"Settings/Tank/3/Capacity"\
			},\
			{\
				DEFAULT_FLUID_TYPE,\
				MIN_OF_FLUID_TYPE,\
				MAX_OF_FLUID_TYPE,\
				"Settings/Tank/3/FluidType"\
			},\
			{\
				european_std,\
				european_std,\
				(num_of_stds - 1),\
				"Settings/Tank/3/Standard"\
			}\
		}\
	},\
	{\
		temperature_t,\
		veFalse,\
		{\
			adc_pin5,\
			INIT_ADC_SAMPLE_VAL,\
			{{},{TEMPERATURE_SENSOR_IIR_LPF_FF_VALUE, TEMPERATURE_SENSOR_CUTOFF_FREQ, INIT_ADC_SAMPLE_MEMORY_VAL}},\
			{\
				"com.victronenergy.temperature.builtin_adc5_di0",\
				veFalse\
			}\
		},\
		{\
			{\
				default_function,\
				no_function,\
				(num_of_functions-1),\
				"Settings/AnalogInput/Temperature/1/Function"\
			},\
			{\
				TEMPERATURE_SCALE,\
				MIN_OF_TEMPERATURE_SCALE,\
				MAX_OF_TEMPERATURE_SCALE,\
				"Settings/Temperature/1/Scale"\
			},\
			{\
				TEMPERATURE_OFFSET,\
				MIN_OF_TEMPERATURE_OFFSET,\
				MAX_OF_TEMPERATURE_OFFSET,\
				"Settings/Temperature/1/Offset"\
			},\
			{\
				DEFAULT_TEMPERATURE_TYPE,\
				MIN_TEMPERATURE_TYPE,\
				(num_of_temperature_sensor_type-1),\
				"Settings/Temperature/1/TemperatureType"\
			}\
		}\
	},\
	{\
		temperature_t,\
		veFalse,\
		{\
			adc_pin3,\
			INIT_ADC_SAMPLE_VAL,\
			{{},{TEMPERATURE_SENSOR_IIR_LPF_FF_VALUE, TEMPERATURE_SENSOR_CUTOFF_FREQ, INIT_ADC_SAMPLE_MEMORY_VAL}},\
			{\
				"com.victronenergy.temperature.builtin_adc3_di1",\
				veFalse\
			}\
		},\
		{\
			{\
				default_function,\
				no_function,\
				(num_of_functions-1),\
				"Settings/AnalogInput/Temperature/2/Function"\
			},\
			{\
				TEMPERATURE_SCALE,\
				MIN_OF_TEMPERATURE_SCALE,\
				MAX_OF_TEMPERATURE_SCALE,\
				"Settings/Temperature/2/Scale"\
			},\
			{\
				TEMPERATURE_OFFSET,\
				MIN_OF_TEMPERATURE_OFFSET,\
				MAX_OF_TEMPERATURE_OFFSET,\
				"Settings/Temperature/2/Offset"\
			},\
			{\
				DEFAULT_TEMPERATURE_TYPE,\
				MIN_TEMPERATURE_TYPE,\
				(num_of_temperature_sensor_type-1),\
				"Settings/Temperature/2/TemperatureType"\
			}\
		}\
	}\
}

typedef enum {
	Connected_item = 0,
	ProductName_item = 1,
	productId_item = 2,
	deviceInstance_item = 3,
	level_item = 4,
	temperature_item = 4,
	remaining_item = 5,
	status_item = 6,
	analogpinFunc_item = 7,
	tank_level_sens_capacity_item = 8,
	temp_sens_Scale_item = 8,
	tank_level_sens_fluidType_item = 9,
	temp_sens_offset_item = 9,
	tank_level_sens_standard_item = 10,
	temp_sens_type_item = 10,
	num_of_container_items = 11
} sensor_items_container_items_t;

// preload the pointer container
#define SENSOR_ITEM_CONTAINER \
{\
	{\
		{&analog_sensor[index_tankLevel1].items.product.connected,										NULL,											"Connected",		&units,	0},\
		{&analog_sensor[index_tankLevel1].items.product.name,											NULL,											"ProductName",		&units,	0},\
		{&analog_sensor[index_tankLevel1].items.product.id,												NULL,											"ProductId",		&units,	0},\
		{&analog_sensor[index_tankLevel1].items.product.instance,										NULL,											"DeviceInstance",	&units,	0},\
		{&analog_sensor[index_tankLevel1].items.tank_level.level,				&analog_sensor[index_tankLevel1].variant.tank_level.level,				"Level",  			&units,	5},\
		{&analog_sensor[index_tankLevel1].items.tank_level.remaining,			&analog_sensor[index_tankLevel1].variant.tank_level.remaining,			"Remaining",  		&units,	5},\
		{&analog_sensor[index_tankLevel1].items.tank_level.status,				&analog_sensor[index_tankLevel1].variant.tank_level.status,				"Status",  			&statusFormat,	5},\
		{&analog_sensor[index_tankLevel1].items.tank_level.analogpinFunc,		&analog_sensor[index_tankLevel1].variant.tank_level.analogpinFunc,		"analogpinFunc",	&units,	5, analogPinFuncChange},\
		{&analog_sensor[index_tankLevel1].items.tank_level.capacity,			&analog_sensor[index_tankLevel1].variant.tank_level.capacity,			"Capacity",			&units,	5, capacityChange},\
		{&analog_sensor[index_tankLevel1].items.tank_level.fluidType,			&analog_sensor[index_tankLevel1].variant.tank_level.fluidType,			"FluidType",  		&fluidTypeFormat,	5, fluidTypeChange},\
		{&analog_sensor[index_tankLevel1].items.tank_level.standard,			&analog_sensor[index_tankLevel1].variant.tank_level.standard,			"Standard",  		&standardFormat,	5, standardChange}\
	},\
	{\
		{&analog_sensor[index_tankLevel2].items.product.connected,										NULL,											"Connected",		&units,	0},\
		{&analog_sensor[index_tankLevel2].items.product.name,											NULL,											"ProductName",		&units,	0},\
		{&analog_sensor[index_tankLevel2].items.product.id,												NULL,											"ProductId",		&units,	0},\
		{&analog_sensor[index_tankLevel2].items.product.instance,										NULL,											"DeviceInstance",	&units,	0},\
		{&analog_sensor[index_tankLevel2].items.tank_level.level,				&analog_sensor[index_tankLevel2].variant.tank_level.level,				"Level",  			&units,	5},\
		{&analog_sensor[index_tankLevel2].items.tank_level.remaining,			&analog_sensor[index_tankLevel2].variant.tank_level.remaining,			"Remaining",  		&units,	5},\
		{&analog_sensor[index_tankLevel2].items.tank_level.status,				&analog_sensor[index_tankLevel2].variant.tank_level.status,				"Status",  			&statusFormat,	5},\
		{&analog_sensor[index_tankLevel2].items.tank_level.analogpinFunc,		&analog_sensor[index_tankLevel2].variant.tank_level.analogpinFunc,		"analogpinFunc",	&units,	5, analogPinFuncChange},\
		{&analog_sensor[index_tankLevel2].items.tank_level.capacity,			&analog_sensor[index_tankLevel2].variant.tank_level.capacity,			"Capacity",			&units,	5, capacityChange},\
		{&analog_sensor[index_tankLevel2].items.tank_level.fluidType,			&analog_sensor[index_tankLevel2].variant.tank_level.fluidType,			"FluidType",  		&fluidTypeFormat,	5, fluidTypeChange},\
		{&analog_sensor[index_tankLevel2].items.tank_level.standard,			&analog_sensor[index_tankLevel2].variant.tank_level.standard,			"Standard",  		&standardFormat,	5, standardChange}\
	},\
	{\
		{&analog_sensor[index_tankLevel3].items.product.connected,										NULL,											"Connected",		&units,	0},\
		{&analog_sensor[index_tankLevel3].items.product.name,											NULL,											"ProductName",		&units,	0},\
		{&analog_sensor[index_tankLevel3].items.product.id,												NULL,											"ProductId",		&units,	0},\
		{&analog_sensor[index_tankLevel3].items.product.instance,										NULL,											"DeviceInstance",	&units,	0},\
		{&analog_sensor[index_tankLevel3].items.tank_level.level,				&analog_sensor[index_tankLevel3].variant.tank_level.level,				"Level",  			&units,	5},\
		{&analog_sensor[index_tankLevel3].items.tank_level.remaining,			&analog_sensor[index_tankLevel3].variant.tank_level.remaining,			"Remaining",  		&units,	5},\
		{&analog_sensor[index_tankLevel3].items.tank_level.status,				&analog_sensor[index_tankLevel3].variant.tank_level.status,				"Status",  			&statusFormat,	5},\
		{&analog_sensor[index_tankLevel3].items.tank_level.analogpinFunc,		&analog_sensor[index_tankLevel3].variant.tank_level.analogpinFunc,		"analogpinFunc",	&units,	5, analogPinFuncChange},\
		{&analog_sensor[index_tankLevel3].items.tank_level.capacity,			&analog_sensor[index_tankLevel3].variant.tank_level.capacity,			"Capacity",			&units,	5, capacityChange},\
		{&analog_sensor[index_tankLevel3].items.tank_level.fluidType,			&analog_sensor[index_tankLevel3].variant.tank_level.fluidType,			"FluidType",  		&fluidTypeFormat,	5, fluidTypeChange},\
		{&analog_sensor[index_tankLevel3].items.tank_level.standard,			&analog_sensor[index_tankLevel3].variant.tank_level.standard,			"Standard",  		&standardFormat,	5, standardChange}\
	},\
	{\
		{&analog_sensor[index_temperature1].items.product.connected,									NULL,											"Connected",		&units,	0},\
		{&analog_sensor[index_temperature1].items.product.name,											NULL,											"ProductName",		&units,	0},\
		{&analog_sensor[index_temperature1].items.product.id,											NULL,											"ProductId",		&units,	0},\
		{&analog_sensor[index_temperature1].items.product.instance,										NULL,											"DeviceInstance",	&units,	0},\
		{&analog_sensor[index_temperature1].items.temperature.temperature,		&analog_sensor[index_temperature1].variant.temperature.temperature,		"Temperature",		&units,	5},\
		{					   NULL,																   NULL,											NULL,		   NULL,	NULL},\
		{&analog_sensor[index_temperature1].items.temperature.status,			&analog_sensor[index_temperature1].variant.temperature.status,			"Status",  			&statusFormat,	5},\
		{&analog_sensor[index_temperature1].items.temperature.analogpinFunc,	&analog_sensor[index_temperature1].variant.temperature.analogpinFunc,   "analogpinFunc",	&units,	5, analogPinFuncChange},\
		{&analog_sensor[index_temperature1].items.temperature.scale,			&analog_sensor[index_temperature1].variant.temperature.scale,			"Scale",			&units,	5, scaleChange},\
		{&analog_sensor[index_temperature1].items.temperature.offset,			&analog_sensor[index_temperature1].variant.temperature.offset,			"Offset",			&units,	5, offsetChange},\
		{&analog_sensor[index_temperature1].items.temperature.temperatureType,	&analog_sensor[index_temperature1].variant.temperature.temperatureType,	"TemperatureType",	&units,	5, TempTypeChange}\
	},\
	{\
		{&analog_sensor[index_temperature2].items.product.connected,									NULL,											"Connected",		&units,	0},\
		{&analog_sensor[index_temperature2].items.product.name,											NULL,											"ProductName",		&units,	0},\
		{&analog_sensor[index_temperature2].items.product.id,											NULL,											"ProductId",		&units,	0},\
		{&analog_sensor[index_temperature2].items.product.instance,										NULL,											"DeviceInstance",	&units,	0},\
		{&analog_sensor[index_temperature2].items.temperature.temperature,		&analog_sensor[index_temperature2].variant.temperature.temperature,		"Temperature",		&units,	5},\
		{					   NULL,																   NULL,											NULL,		   NULL,	NULL},\
		{&analog_sensor[index_temperature2].items.temperature.status,			&analog_sensor[index_temperature2].variant.temperature.status,			"Status",  			&statusFormat,	5},\
		{&analog_sensor[index_temperature2].items.temperature.analogpinFunc,	&analog_sensor[index_temperature2].variant.temperature.analogpinFunc,	"analogpinFunc",	&units,	5, analogPinFuncChange},\
		{&analog_sensor[index_temperature2].items.temperature.scale,			&analog_sensor[index_temperature2].variant.temperature.scale,			"Scale",			&units,	5, scaleChange},\
		{&analog_sensor[index_temperature2].items.temperature.offset,			&analog_sensor[index_temperature2].variant.temperature.offset,			"Offset",			&units,	5, offsetChange},\
		{&analog_sensor[index_temperature2].items.temperature.temperatureType,	&analog_sensor[index_temperature2].variant.temperature.temperatureType,	"TemperatureType",	&units,	5, TempTypeChange}\
	}\
}
#endif // End of sensors.h file
