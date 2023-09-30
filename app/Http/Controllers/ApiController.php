<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use DB; // Or use Eloquent Models if you have them
use App\Models\CheckIn; // Import the CheckIn model
use App\Models\Agent; // Import the Agent model

class ApiController extends Controller
{


    public function checkIn(Request $request)
    {
        $data = $request->validate([
            'uuid' => 'required|uuid',
            'name' => 'required|string',
        ]);

        CheckIn::create($data);

        return response()->json(['status' => 'success'], 200);
    }



    public function agent(Request $request)
    {
        $data = $request->validate([
            'uuid' => 'required|uuid',
            'name' => 'required|string',
            'ip' => 'required|ip',
            'asn' => 'nullable|string',
            'hostname' => 'nullable|string',
            'city' => 'nullable|string',
            'post_code' => 'nullable|string',
            'country' => 'nullable|string',
            'country_code' => 'nullable|string',
            'latitude' => 'nullable|numeric',
            'longitude' => 'nullable|numeric',
        ]);

        Agent::create($data);

        return response()->json(['status' => 'success'], 201);
    }
}
