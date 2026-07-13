import subprocess
import json
import os
from http.server import BaseHTTPRequestHandler, HTTPServer

class AeroBridgeHandler(BaseHTTPRequestHandler):
    
    def _set_cors_headers(self, status=200):
        self.send_response(status)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'POST, GET, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type, Authorization')
        self.end_headers()

    def do_OPTIONS(self):
       
        self._set_cors_headers(200)

    def do_POST(self):
        if self.path == '/run_cpp_engine':
            content_length = int(self.headers['Content-Length'])
            post_data = json.loads(self.rfile.read(content_length).decode('utf-8'))
            
            
            executable_name = "UAV_workspace_Engine.exe" if os.name == 'nt' else "./UAV_workspace_Engine"
            
            cmd = [
                executable_name,
                str(post_data.get('total_takeoff_weight_g', 700)),
                str(post_data.get('wing_loading', 37.5)),
                str(post_data.get('main_aspect_ratio', 9)),
                str(post_data.get('total_moment_arm', 52) / 100.0),
                str(post_data.get('horiz_tail_vol_coeff', 0.5)),
                str(post_data.get('horiz_aspect_ratio', 4)),
                str(post_data.get('vert_tail_vol_coeff', 0.04)),
                str(post_data.get('vert_aspect_ratio', 1.6)),
                str(post_data.get('max_lift_coeff', 1.5)),      
                str(post_data.get('angle_of_attack', 5)),
                str(post_data.get('wing_tunnel_velocity', 15)),
                str(post_data.get('payload_balance_shift', 0))
            ]

            try:
              
                result = subprocess.run(cmd, capture_output=True, text=True, check=True)
                response_data = result.stdout.encode('utf-8')
                status_code = 200
            except subprocess.CalledProcessError as e:
               
                error_msg = {"error": "C++ Runtime Crash", "details": str(e.stderr)}
                response_data = json.dumps(error_msg).encode('utf-8')
                status_code = 500
            except FileNotFoundError:
               
                error_msg = {"error": f"Executable not found at path: {executable_name}. Ensure it is compiled and placed inside this directory."}
                response_data = json.dumps(error_msg).encode('utf-8')
                status_code = 500
            
           
            self._set_cors_headers(status_code)
            self.wfile.write(response_data)
        else:
            self.send_response(404)
            self.end_headers()

# Server Activation
if __name__ == '__main__':
    # Binding on multi-address interface layer to circumvent strict cross-origin restrictions
    print("UAV Aero Engine Bridge active on http://localhost:8080")
    try:
        HTTPServer(('0.0.0.0', 8080), AeroBridgeHandler).serve_forever()
    except KeyboardInterrupt:
        print("\nServer shutting down cleanly.")