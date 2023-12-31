<?php

use Illuminate\Support\Facades\Route;
use App\Http\Controllers\LoginController;
use App\Http\Controllers\DashboardController;
use App\Http\Controllers\BuilderController;
use App\Http\Controllers\CompileController;

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider and all of them will
| be assigned to the "web" middleware group. Make something great!
|
*/

Route::get('/', function () {
    if (session('authenticated')) {
        return redirect('/dashboard');
    }

    return redirect('/login');
});

Route::get('login', [LoginController::class, 'showLoginForm'])->name('login.form');
Route::post('login', [LoginController::class, 'login'])->name('login');

Route::post('/logout', [LoginController::class, 'logout'])->name('logout');

Route::middleware(['authenticated'])->group(function () {
    // Protected Routes Here
    Route::get('/dashboard', [DashboardController::class, 'index'])->name('dashboard');
    Route::get('/dashboard/builder', [BuilderController::class, 'showBuilder'])->name('builder');
    Route::post('/dashboard/builder', [CompileController::class, 'generateAgent'])->name('compile');
});
