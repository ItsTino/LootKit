<?php

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;

/*
|--------------------------------------------------------------------------
| API Routes
|--------------------------------------------------------------------------
|
| Here is where you can register API routes for your application. These
| routes are loaded by the RouteServiceProvider and all of them will
| be assigned to the "api" middleware group. Make something great!
|
*/

// Define the check-in route
Route::post('/api/check-in', 'ApiController@checkIn');

// Define the agent route
Route::post('/api/agent', 'ApiController@agent');

Route::middleware('auth:sanctum')->get('/user', function (Request $request) {
    return $request->user();
});
