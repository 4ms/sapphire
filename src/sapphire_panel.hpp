// *** GENERATED CODE *** DO NOT EDIT ***
#pragma once
#include <map>
#include <string>

namespace Sapphire
{
    struct ComponentLocation
    {
        float cx;     // x-coordinate of control's center
        float cy;     // y-coordinate of control's center

        ComponentLocation(float _cx, float _cy)
            : cx(_cx)
            , cy(_cy)
            {}
    };

    using ComponentMap = std::map<std::string, ComponentLocation>;
    using ModuleMap = std::map<std::string, ComponentMap>;

    class ComponentPlacer
    {
    private:
        ModuleMap modmap
        {
            { "elastika", {
                {"audio_left_input",       {   7.500,  115.000}},
                {"audio_left_output",      {  40.460,  115.000}},
                {"audio_right_input",      {  20.500,  115.000}},
                {"audio_right_output",     {  53.460,  115.000}},
                {"curl_atten",             {  41.720,   72.000}},
                {"curl_cv",                {  41.720,   81.740}},
                {"curl_slider",            {  41.720,   46.000}},
                {"drive_knob",             {  14.000,  102.000}},
                {"fric_atten",             {   8.000,   72.000}},
                {"fric_cv",                {   8.000,   81.740}},
                {"fric_slider",            {   8.000,   46.000}},
                {"input_tilt_atten",       {   8.000,   12.500}},
                {"input_tilt_cv",          {   8.000,   22.500}},
                {"input_tilt_knob",        {  19.240,   17.500}},
                {"level_knob",             {  46.960,  102.000}},
                {"mass_atten",             {  52.960,   72.000}},
                {"mass_cv",                {  52.960,   81.740}},
                {"mass_slider",            {  52.960,   46.000}},
                {"output_tilt_atten",      {  53.000,   12.500}},
                {"output_tilt_cv",         {  53.000,   22.500}},
                {"output_tilt_knob",       {  41.720,   17.500}},
                {"power_gate_input",       {  30.480,  104.000}},
                {"power_toggle",           {  30.480,   95.000}},
                {"span_atten",             {  30.480,   72.000}},
                {"span_cv",                {  30.480,   81.740}},
                {"span_slider",            {  30.480,   46.000}},
                {"stif_atten",             {  19.240,   72.000}},
                {"stif_cv",                {  19.240,   81.740}},
                {"stif_slider",            {  19.240,   46.000}},
            }},
            { "frolic", {
                {"chaos_atten",            {   5.160,   68.000}},
                {"chaos_cv",               {  15.160,   68.000}},
                {"chaos_knob",             {  10.160,   57.000}},
                {"p_output",               {  10.160,  115.000}},
                {"speed_atten",            {   5.160,   37.000}},
                {"speed_cv",               {  15.160,   37.000}},
                {"speed_knob",             {  10.160,   26.000}},
                {"x_output",               {  10.160,   88.000}},
                {"y_output",               {  10.160,   97.000}},
                {"z_output",               {  10.160,  106.000}},
            }},
            { "galaxy", {
                {"audio_left_input",       {   9.240,   22.000}},
                {"audio_left_output",      {   9.240,  114.000}},
                {"audio_right_input",      {  21.240,   22.000}},
                {"audio_right_output",     {  21.240,  114.000}},
                {"bigness_atten",          {  15.240,   83.333}},
                {"bigness_cv",             {   6.240,   83.333}},
                {"bigness_knob",           {  24.240,   83.333}},
                {"brightness_atten",       {  15.240,   52.667}},
                {"brightness_cv",          {   6.240,   52.667}},
                {"brightness_knob",        {  24.240,   52.667}},
                {"detune_atten",           {  15.240,   68.000}},
                {"detune_cv",              {   6.240,   68.000}},
                {"detune_knob",            {  24.240,   68.000}},
                {"mix_atten",              {  15.240,   98.667}},
                {"mix_cv",                 {   6.240,   98.667}},
                {"mix_knob",               {  24.240,   98.667}},
                {"replace_atten",          {  15.240,   37.333}},
                {"replace_cv",             {   6.240,   37.333}},
                {"replace_knob",           {  24.240,   37.333}},
            }},
            { "glee", {
                {"chaos_atten",            {   5.160,   68.000}},
                {"chaos_cv",               {  15.160,   68.000}},
                {"chaos_knob",             {  10.160,   57.000}},
                {"p_output",               {  10.160,  115.000}},
                {"speed_atten",            {   5.160,   37.000}},
                {"speed_cv",               {  15.160,   37.000}},
                {"speed_knob",             {  10.160,   26.000}},
                {"x_output",               {  10.160,   88.000}},
                {"y_output",               {  10.160,   97.000}},
                {"z_output",               {  10.160,  106.000}},
            }},
            { "gravy", {
                {"audio_left_input",       {   9.240,   22.000}},
                {"audio_left_output",      {   9.240,  114.000}},
                {"audio_right_input",      {  21.240,   22.000}},
                {"audio_right_output",     {  21.240,  114.000}},
                {"frequency_atten",        {  15.240,   40.400}},
                {"frequency_cv",           {   6.240,   40.400}},
                {"frequency_knob",         {  24.240,   40.400}},
                {"gain_atten",             {  15.240,   95.600}},
                {"gain_cv",                {   6.240,   95.600}},
                {"gain_knob",              {  24.240,   95.600}},
                {"mix_atten",              {  15.240,   77.200}},
                {"mix_cv",                 {   6.240,   77.200}},
                {"mix_knob",               {  24.240,   77.200}},
                {"resonance_atten",        {  15.240,   58.800}},
                {"resonance_cv",           {   6.240,   58.800}},
                {"resonance_knob",         {  24.240,   58.800}},
            }},
            { "hiss", {
                {"random_output_1",        {   7.620,   18.000}},
                {"random_output_10",       {   7.620,  112.000}},
                {"random_output_2",        {   7.620,   28.444}},
                {"random_output_3",        {   7.620,   38.889}},
                {"random_output_4",        {   7.620,   49.333}},
                {"random_output_5",        {   7.620,   59.778}},
                {"random_output_6",        {   7.620,   70.222}},
                {"random_output_7",        {   7.620,   80.667}},
                {"random_output_8",        {   7.620,   91.111}},
                {"random_output_9",        {   7.620,  101.556}},
            }},
            { "nucleus", {
                {"audio_mode_button",      {  15.640,   83.500}},
                {"decay_atten",            {  35.640,   36.000}},
                {"decay_cv",               {  45.640,   36.000}},
                {"decay_knob",             {  40.640,   25.000}},
                {"in_drive_atten",         {  60.640,   66.500}},
                {"in_drive_cv",            {  70.640,   66.500}},
                {"in_drive_knob",          {  65.640,   55.500}},
                {"magnet_atten",           {  60.640,   36.000}},
                {"magnet_cv",              {  70.640,   36.000}},
                {"magnet_knob",            {  65.640,   25.000}},
                {"out_level_atten",        {  10.640,  114.000}},
                {"out_level_cv",           {  20.640,  114.000}},
                {"out_level_knob",         {  15.640,  103.000}},
                {"speed_atten",            {  10.640,   36.000}},
                {"speed_cv",               {  20.640,   36.000}},
                {"speed_knob",             {  15.640,   25.000}},
                {"x1_output",              {  40.640,   86.000}},
                {"x2_output",              {  40.640,   94.667}},
                {"x3_output",              {  40.640,  103.333}},
                {"x4_output",              {  40.640,  112.000}},
                {"x_input",                {  16.140,   58.000}},
                {"y1_output",              {  53.140,   86.000}},
                {"y2_output",              {  53.140,   94.667}},
                {"y3_output",              {  53.140,  103.333}},
                {"y4_output",              {  53.140,  112.000}},
                {"y_input",                {  28.640,   58.000}},
                {"z1_output",              {  65.640,   86.000}},
                {"z2_output",              {  65.640,   94.667}},
                {"z3_output",              {  65.640,  103.333}},
                {"z4_output",              {  65.640,  112.000}},
                {"z_input",                {  41.140,   58.000}},
            }},
            { "pivot", {
                {"a_input",                {  10.160,   22.000}},
                {"c_output",               {  10.160,  115.000}},
                {"twist_atten",            {   5.160,   68.000}},
                {"twist_cv",               {  15.160,   68.000}},
                {"twist_knob",             {  10.160,   57.000}},
                {"x_output",               {  10.160,   88.000}},
                {"y_output",               {  10.160,   97.000}},
                {"z_output",               {  10.160,  106.000}},
            }},
            { "polynucleus", {
                {"a_input",                {  15.640,   50.000}},
                {"audio_mode_button",      {  15.640,   66.000}},
                {"b_output",               {  65.640,   81.000}},
                {"c_output",               {  65.640,   90.667}},
                {"clear_button",           {  15.640,  110.000}},
                {"d_output",               {  65.640,  100.333}},
                {"decay_atten",            {  35.640,   36.000}},
                {"decay_cv",               {  45.640,   36.000}},
                {"decay_knob",             {  40.640,   25.000}},
                {"e_output",               {  65.640,  110.000}},
                {"in_drive_atten",         {  35.640,   66.500}},
                {"in_drive_cv",            {  45.640,   66.500}},
                {"in_drive_knob",          {  40.640,   55.500}},
                {"magnet_atten",           {  60.640,   36.000}},
                {"magnet_cv",              {  70.640,   36.000}},
                {"magnet_knob",            {  65.640,   25.000}},
                {"out_level_atten",        {  60.640,   66.500}},
                {"out_level_cv",           {  70.640,   66.500}},
                {"out_level_knob",         {  65.640,   55.500}},
                {"speed_atten",            {  10.640,   36.000}},
                {"speed_cv",               {  20.640,   36.000}},
                {"speed_knob",             {  15.640,   25.000}},
            }},
            { "pop", {
                {"chaos_atten",            {   5.160,   68.000}},
                {"chaos_cv",               {  15.160,   68.000}},
                {"chaos_knob",             {  10.160,   57.000}},
                {"pulse_output",           {  10.160,  115.000}},
                {"speed_atten",            {   5.160,   37.000}},
                {"speed_cv",               {  15.160,   37.000}},
                {"speed_knob",             {  10.160,   26.000}},
                {"sync_input",             {  10.160,   95.000}},
            }},
            { "rotini", {
                {"a_input",                {  10.160,   22.000}},
                {"b_input",                {  10.160,   36.667}},
                {"c_output",               {  10.160,  115.000}},
                {"x_output",               {  10.160,   88.000}},
                {"y_output",               {  10.160,   97.000}},
                {"z_output",               {  10.160,  106.000}},
            }},
            { "sam", {
                {"p_input",                {   5.080,   52.000}},
                {"p_output",               {   5.080,  115.000}},
                {"x_input",                {   5.080,   25.000}},
                {"x_output",               {   5.080,   88.000}},
                {"y_input",                {   5.080,   34.000}},
                {"y_output",               {   5.080,   97.000}},
                {"z_input",                {   5.080,   43.000}},
                {"z_output",               {   5.080,  106.000}},
            }},
            { "tin", {
                {"clear_trigger_input",    {  10.160,  110.000}},
                {"level_atten",            {   5.160,   86.000}},
                {"level_cv",               {  15.160,   86.000}},
                {"level_knob",             {  10.160,   75.000}},
                {"p_input",                {  10.160,   55.000}},
                {"x_input",                {  10.160,   25.000}},
                {"y_input",                {  10.160,   35.000}},
                {"z_input",                {  10.160,   45.000}},
            }},
            { "tout", {
                {"clear_trigger_output",   {  10.160,  110.000}},
                {"level_atten",            {   5.160,   86.000}},
                {"level_cv",               {  15.160,   86.000}},
                {"level_knob",             {  10.160,   75.000}},
                {"p_output",               {  10.160,   55.000}},
                {"x_output",               {  10.160,   25.000}},
                {"y_output",               {  10.160,   35.000}},
                {"z_output",               {  10.160,   45.000}},
            }},
        };

    public:
        ComponentLocation find(const std::string& modCode, const std::string& label)
        {
            auto m = modmap.find(modCode);
            if (m == modmap.end()) return ComponentLocation{0, 0};
            auto c = m->second.find(label);
            if (c == m->second.end()) return ComponentLocation{0, 0};
            return c->second;
        }
    };

    extern ComponentPlacer TheComponentPlacer;
}